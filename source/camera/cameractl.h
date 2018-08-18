#ifndef CAMERACTL_H
#define CAMERACTL_H

#include <QObject>
#include <QImage>
#include <cv.h>
#include <highgui.h>
using namespace cv;


class CameraCtl : public QObject
{
    Q_OBJECT
public:
    explicit CameraCtl(QObject *parent = 0);

    virtual bool Initiallize(int DeviceNumber,QString paramPath = "")=0;//打开相机，并设置相机参数

    virtual bool StartView()=0;//开始采集
    virtual bool StopView()=0;//停止采集并且关闭相机

    virtual Mat GrabOneFrame()=0 ;// 图片抓取(单次触发模式下，触发采集)

    virtual int GetCamWidth()=0;//获取当前相机所设宽度
    virtual int GetCamHeight()=0;//获取当前相机所设高度
    virtual void SetPara()=0;//相机参数控制

signals:
    void hasImage(const QImage& image);
};

#endif // CAMERACTL_H
