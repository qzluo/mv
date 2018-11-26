#include "qdahengcameractl.h"

#include "FileLogger.h"
#include <QDebug>
#include <QMutexLocker>

void CSampleCaptureEventHandler::DoOnImageCaptured(CImageDataPointer &objImageDataPointer, void* pUserParam)
{
    if (objImageDataPointer->GetStatus() == GX_FRAME_STATUS_SUCCESS) {
        GX_PIXEL_FORMAT_ENTRY emPixelFormat = objImageDataPointer->GetPixelFormat();

        void* pRGB24Buffer = NULL;

        if ((emPixelFormat & GX_PIXEL_12BIT) == GX_PIXEL_12BIT)
            pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_4_11, GX_RAW2RGB_NEIGHBOUR, true);
        else if ((emPixelFormat & GX_PIXEL_10BIT) == GX_PIXEL_10BIT)
            pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_2_9, GX_RAW2RGB_NEIGHBOUR, true);
        else if ((emPixelFormat & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
            pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);

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
    camWidth = 1000;
    camHeight = 1000;
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
//    gxdeviceinfo_vector vectorDeviceInfo;
//    IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
//    if (0 == vectorDeviceInfo.size())
//        return false;

    return true;
}

bool QDahengCameraCtl::StartView()
{
    //枚举设备
    gxdeviceinfo_vector vectorDeviceInfo;
    IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
    if (0 == vectorDeviceInfo.size()) {
        logFile(FileLogger::warn, "Failed to start daheng camera.");
        return false;
    }

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
    QString dahengFeatureName = getDahengCamFeatureName(featureName);
    if (dahengFeatureName.isEmpty())
        return -1;

    char msg[1024] = {};

    //获取远端设备属性控制器
    CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

    if (!ObjFeatureControlPtr->IsImplemented(dahengFeatureName.toLatin1().data())) {
        sprintf(msg, "Feature %s is not implemented.", dahengFeatureName.toLatin1().data());
        logFile(FileLogger::warn, msg);
        return -1;
    }

    //check flags
    if (!ObjFeatureControlPtr->IsWritable(dahengFeatureName.toLatin1().data())) {
        sprintf(msg, "Feature %s is not writable.", dahengFeatureName.toLatin1().data());
        logFile(FileLogger::warn, msg);
        return -1;
    }

    //get feature type
    GX_FEATURE_TYPE dataType = ObjFeatureControlPtr->
            GetFeatureType(dahengFeatureName.toLatin1().data());

    switch (dataType) {
    case GX_FEATURE_INT:
    {
        CIntFeaturePointer objIntPtr = ObjFeatureControlPtr->GetIntFeature(dahengFeatureName.toLatin1().data());
        int iMin = 0;
        int iMax = 0;
        int iValue = value.toInt();

        iMin = objIntPtr->GetMin();
        iMax = objIntPtr->GetMax();

        if (iValue < iMin)
            iValue = iMin;
        else if (iValue > iMax)
            iValue = iMax;

        objIntPtr->SetValue(iValue);
        break;
    }

    case GX_FEATURE_FLOAT:
    {
        CFloatFeaturePointer objFloatPtr = ObjFeatureControlPtr->GetFloatFeature(dahengFeatureName.toLatin1().data());
        double dMin = 0.0;
        double dMax = 0.0;
        double dValue = value.toDouble();

        dMin = objFloatPtr->GetMin();
        dMax = objFloatPtr->GetMax();

        objFloatPtr->SetValue(dValue);
        break;
    }

    case GX_FEATURE_ENUM:
    {
        CEnumFeaturePointer objEnumPtr = ObjFeatureControlPtr->GetEnumFeature(dahengFeatureName.toLatin1().data());
        QString sCurrentValue = objEnumPtr->GetValue();

        if (value.compare(sCurrentValue) == 0)
            return 0;

        objEnumPtr->SetValue(value.toStdString().c_str());
        break;
    }

    case GX_FEATURE_BOOL:
    case GX_FEATURE_STRING:
    case GX_FEATURE_BUFFER:
    case GX_FEATURE_COMMAND:
    default:
        break;
    }

    return 0;
}

int QDahengCameraCtl::GetPara(QString featureName, CameraFeature *pCamearaFeature)
{
    QString dahengFeatureName = getDahengCamFeatureName(featureName);
    if (dahengFeatureName.isEmpty())
        return -1;

    char msg[1024] = {};

    //获取远端设备属性控制器
    CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

    if (!ObjFeatureControlPtr->IsImplemented(dahengFeatureName.toLatin1().data())) {
        sprintf(msg, "Feature %s is not implemented.", dahengFeatureName.toLatin1().data());
        logFile(FileLogger::warn, msg);
        return -1;
    }

    //check flags
    if (!ObjFeatureControlPtr->IsReadable(dahengFeatureName.toLatin1().data())) {
        sprintf(msg, "Feature %s is not readable.", dahengFeatureName.toLatin1().data());
        logFile(FileLogger::warn, msg);
        return -1;
    }

    //get feature type
    GX_FEATURE_TYPE dataType = ObjFeatureControlPtr->
            GetFeatureType(dahengFeatureName.toLatin1().data());

    switch (dataType) {
    case GX_FEATURE_INT:
    {
        pCamearaFeature->dataType = CamFeatureDataInt;
        CIntFeaturePointer objIntPtr = ObjFeatureControlPtr->GetIntFeature(dahengFeatureName.toLatin1().data());
        int iMin = 0;
        int iMax = 0;
        int iValue = 0;

        iMin = objIntPtr->GetMin();
        iMax = objIntPtr->GetMax();
        iValue = objIntPtr->GetValue();

        pCamearaFeature->min = QString::number(iMin);
        pCamearaFeature->max = QString::number(iMax);
        pCamearaFeature->value = QString::number(iValue);
        break;
    }

    case GX_FEATURE_FLOAT:
    {
        pCamearaFeature->dataType = CamFeatureDataFloat;
        CFloatFeaturePointer objFloatPtr = ObjFeatureControlPtr->GetFloatFeature(dahengFeatureName.toLatin1().data());
        double dMin = 0.0;
        double dMax = 0.0;
        double dValue = 0.0;

        dMin = objFloatPtr->GetMin();
        dMax = objFloatPtr->GetMax();
        dValue = objFloatPtr->GetValue();

        pCamearaFeature->min = QString::number(dMin);
        pCamearaFeature->max = QString::number(dMax);
        pCamearaFeature->value = QString::number(dValue);
        break;
    }

    case GX_FEATURE_ENUM:
    {
        pCamearaFeature->dataType = CamFeatureDataEnum;
        CEnumFeaturePointer objEnumPtr = ObjFeatureControlPtr->GetEnumFeature(dahengFeatureName.toLatin1().data());
        GxIAPICPP::gxstring_vector sEnumEntries = objEnumPtr->GetEnumEntryList();
        GxIAPICPP::gxstring sCurrentValue = objEnumPtr->GetValue();

        pCamearaFeature->enumStrings.clear();
        for (int i = 0; i < sEnumEntries.size(); i++) {
            QString entryItem = sEnumEntries.at(i);
            pCamearaFeature->enumStrings.append(entryItem);
        }

        pCamearaFeature->value = sCurrentValue;

        break;
    }

    case GX_FEATURE_BOOL:
    case GX_FEATURE_STRING:
    case GX_FEATURE_BUFFER:
    case GX_FEATURE_COMMAND:
    default:
        break;
    }

    return 0;
}

QString QDahengCameraCtl::getDahengCamFeatureName(QString featureName)
{
    if (featureName == "ExposureTimeAbs")
        return "ExposureTime";
    else if (featureName == "Gain")
        return "Gain";
    else if (featureName == "TriggerMode")
        return "TriggerMode";
    else if (featureName == "TriggerSource")
        return "TriggerSource";
    else if (featureName == "TriggerDelayAbs")
        return "TriggerDelay";
    else if (featureName == "BalanceRatioSelector")
        return "BalanceRatioSelector";
    else if (featureName == "BalanceRatioAbs")
        return "BalanceRatioAbs";
    else
        return "";
}

void QDahengCameraCtl::OnFrameReady()
{
    QImage image;
    if (((CSampleCaptureEventHandler*)pCaptureEventHandler)->GetFrame(image) < 0)
        return ;

    emit hasImage(image);
}
