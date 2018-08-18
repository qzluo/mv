#ifndef QVIRTUALCAMERACTL_H
#define QVIRTUALCAMERACTL_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QMutex>
#include "cameractl.h"

class QVirtualCameraWorker : public QObject
{
    Q_OBJECT

public:
    QVirtualCameraWorker(QObject *parent = 0);
    ~QVirtualCameraWorker();

    QString getFilePath() const;
    void setFilePath(const QString &value);

    QStringList getImgFileNames() const;
    void setImgFileNames(const QStringList &value);

public slots:
    void onStartCapture();
    void onStopCapture();
    void updateCapture();

signals:
    void hasImage(const QImage& image);

private:
    int curImgIndex;
    QStringList imgFileNames;
    QString filePath;

    bool bStarted;
    int capturePeriod;   //应该大于曝光时间, ms

    QMutex mutex;
};

class QVirtualCameraCtl : public CameraCtl
{
    Q_OBJECT
public:
    explicit QVirtualCameraCtl(QObject *parent = 0);
    ~QVirtualCameraCtl();

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
    virtual void SetPara();

signals:
    void startCapture();
    void stopCapture();

private:
    QThread workerThread;
    QVirtualCameraWorker* worker;
    int camWidth;
    int camHeight;

};


#endif // QVIRTUALCAMERACTL_H
