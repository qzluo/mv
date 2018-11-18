#ifndef QDAHENGCAMERACTL_H
#define QDAHENGCAMERACTL_H

#include <QObject>
#include <QImage>
#include <QMutex>

#include "cameractl.h"
#include "GalaxyIncludes.h"

//用户继承采集事件处理类
class CSampleCaptureEventHandler :  public QObject, public ICaptureEventHandler
{
    Q_OBJECT

public:
    void DoOnImageCaptured(CImageDataPointer &objImageDataPointer, void* pUserParam);

    int GetFrame(QImage &image);

signals:
    void frameReceivedSignal();

private:
    QList<QImage> images;
    QMutex mutex;
};

class QDahengCameraCtl : public CameraCtl
{
    Q_OBJECT

public:
    explicit QDahengCameraCtl(QObject *parent = 0);
    ~QDahengCameraCtl();

    virtual bool Initiallize(int DeviceNumber,QString paramPath = "");//打开相机，并设置相机参数

    virtual bool StartView();//开始采集
    virtual bool StopView();//停止采集并且关闭相机

    virtual Mat GrabOneFrame();// 图片抓取(单次触发模式下，触发采集)

    virtual int GetCamWidth();//获取当前相机所设宽度
    virtual int GetCamHeight();//获取当前相机所设高度
    virtual int SetPara(QString featureName, QString value);//相机参数控制

    virtual int GetPara(QString featureName, CameraFeature* pCamearaFeature);

private:
    QString getDahengCamFeatureName(QString featureName);

public slots:
    void OnFrameReady();

private:
    bool bInit;
    int camWidth;
    int camHeight;

    CGXDevicePointer ObjDevicePtr;
    CGXStreamPointer ObjStreamPtr;
    ICaptureEventHandler* pCaptureEventHandler;

};

#endif // QDAHENGCAMERACTL_H
