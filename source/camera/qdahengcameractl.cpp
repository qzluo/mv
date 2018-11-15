#include "qdahengcameractl.h"

#include <QDebug>
#include <QMutexLocker>

void CSampleCaptureEventHandler::DoOnImageCaptured(CImageDataPointer &objImageDataPointer, void* pUserParam)
{
    if (objImageDataPointer->GetStatus() == GX_FRAME_STATUS_SUCCESS) {
        GX_PIXEL_FORMAT_ENTRY emPixelFormat = objImageDataPointer->GetPixelFormat();

        void*pRGB24Buffer = NULL;

        if (emPixelFormat & GX_PIXEL_8BIT)
            pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);
        else if (emPixelFormat & GX_PIXEL_8BIT == 0)
            pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_4_11, GX_RAW2RGB_NEIGHBOUR, true);

        QImage image((uchar*)pRGB24Buffer,
                     objImageDataPointer->GetWidth(),
                     objImageDataPointer->GetHeight(),
                     QImage::Format_RGB888);

        QMutexLocker locker(&mutex);
        images.append(image);

        emit frameReceivedSignal();
    }
}

int CSampleCaptureEventHandler::GetFrame(QImage &image)
{
    QMutexLocker locker(&mutex);

    if (!images.empty()) {
        image = images.front();
        images.pop_front();

        return 0;
    }

    return -1;
}

QDahengCameraCtl::QDahengCameraCtl(QObject *parent) : CameraCtl(parent)
{
    bInit = false;
    camWidth = 0;
    camHeight = 0;
    pCaptureEventHandler = NULL;
}

QDahengCameraCtl::~QDahengCameraCtl()
{
    if (bInit)
        IGXFactory::GetInstance().Uninit();

    if (pCaptureEventHandler) {
        delete pCaptureEventHandler;
        pCaptureEventHandler = NULL;
    }
}

bool QDahengCameraCtl::Initiallize(int DeviceNumber, QString paramPath)
{
    //初始化
    if (!bInit) {
        IGXFactory::GetInstance().Init();
        bInit = true;
    }

    //枚举设备
    gxdeviceinfo_vector vectorDeviceInfo;
    IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
    if (0 == vectorDeviceInfo.size())
        return false;

    return true;
}

bool QDahengCameraCtl::StartView()
{
    //枚举设备
    gxdeviceinfo_vector vectorDeviceInfo;
    IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
    if (0 == vectorDeviceInfo.size())
        return false;

    //打开第一台设备以及设备下面第一个流
    ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
                vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);

    ObjStreamPtr = ObjDevicePtr->OpenStream(0);

    //注册设备掉线事件
//    GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline = NULL;

//    pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();
//    hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, NULL);

    //获取远端设备属性控制器
    CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

    //注册回调采集
    if (!pCaptureEventHandler) {
        pCaptureEventHandler = new CSampleCaptureEventHandler();
        connect((CSampleCaptureEventHandler*)pCaptureEventHandler,
                &CSampleCaptureEventHandler::frameReceivedSignal,
                this, &QDahengCameraCtl::OnFrameReady);
    }

    ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

    //获得相机尺寸
    CIntFeaturePointer objIntPtr = ObjFeatureControlPtr->GetIntFeature("Width");
    camWidth = objIntPtr->GetValue();

    objIntPtr = ObjFeatureControlPtr->GetIntFeature("Height");
    camHeight = objIntPtr->GetValue();

    //发送开采命令
    ObjStreamPtr->StartGrab();
    ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

    return true;
}

bool QDahengCameraCtl::StopView()
{
    //获取远端设备属性控制器
    CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

    //发送停采命令
    ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
    ObjStreamPtr->StopGrab();

    //注销采集回调
    ObjStreamPtr->UnregisterCaptureCallback();

    //注销设备掉线事件
//    ObjDevicePtr->UnregisterDeviceOfflineCallback(hDeviceOffline);

    //释放资源
    ObjStreamPtr->Close();
    ObjDevicePtr->Close();

    return true;
}

Mat QDahengCameraCtl::GrabOneFrame()
{
    Mat image;
    return image;
}

int QDahengCameraCtl::GetCamWidth()
{
    return camWidth;
}

int QDahengCameraCtl::GetCamHeight()
{
    return camHeight;
}

int QDahengCameraCtl::SetPara(QString featureName, QString value)
{
    return 0;
}

int QDahengCameraCtl::GetPara(QString featureName, CameraFeature *pCamearaFeature)
{
    return 0;
}

void QDahengCameraCtl::OnFrameReady()
{
    QImage image;
    if (((CSampleCaptureEventHandler*)pCaptureEventHandler)->GetFrame(image) < 0)
        return ;

    emit hasImage(image);
}
