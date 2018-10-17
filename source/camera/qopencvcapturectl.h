#ifndef QOPENCVCAPTURECTL_H
#define QOPENCVCAPTURECTL_H

#include <QObject>
#include <QThread>
#include <QImage>

#include <cv.h>
#include <highgui.h>
#include <QMutex>
#include "cameractl.h"

class QOpenCVCaptureWorker : public QObject
{
    Q_OBJECT

public:
    QOpenCVCaptureWorker(QObject *parent = 0);
    ~QOpenCVCaptureWorker();

    bool Initiallize(int device, QString paramPath="");
    cv::Mat GrabOneFrame();

    int getCamWidth();
    int getCamHeight();

    void showPropertyPage(int nCamID);

private:
    //由相机名字排序索引,获得相机的默认设备号, -1表示没找到相应的相机
    int getCameraDeviceNumber(int index);

public slots:
    void onStartCapture();
    void onStopCapture();
    void updateCapture();

signals:
    void hasImage(const QImage& image);

private:
    cv::VideoCapture cap;
    cv::Mat mat;
    bool bStartd;
    int capturePeriod;   //应该大于曝光时间, ms

    QMutex mutex;
};

class QOpenCVCaptureCtl : public CameraCtl
{
    Q_OBJECT

public:
    QOpenCVCaptureCtl(QObject *parent = 0);
    ~QOpenCVCaptureCtl();

    //打开相机，并设置相机参数
    virtual bool Initiallize(int DeviceNumber, QString paramPath="");

    //开始采集
    virtual bool StartView();

    //停止采集并且关闭相机
    virtual bool StopView();

    // 图片抓取(单次触发模式下，触发采集)
    virtual cv::Mat GrabOneFrame();

    //获取当前相机所设宽度
    virtual int GetCamWidth();

    //获取当前相机所设高度
    virtual int GetCamHeight();

    //相机参数控制
    virtual int SetPara(QString featureName, QString value);

    virtual int GetPara(QString featureName, CameraFeature* pCamearaFeature);

    void showPropertyPage(int nCamID);


signals:
    void startCapture();
    void stopCapture();

private:
    QThread workerThread;
    QOpenCVCaptureWorker* worker;

};

#endif // QOPENCVCAPTURECTL_H
