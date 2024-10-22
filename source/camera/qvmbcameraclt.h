#ifndef QVMBCAMERACLT_H
#define QVMBCAMERACLT_H

#include <QObject>
#include <QImage>
#include <QMutex>

#include "cameractl.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaC/Include/VimbaC.h"
#include "FrameObserver.h"
#include "Mat2QImage.h"

enum    { NUM_FRAMES=3, };

class QVmbCameraclt : public CameraCtl
{
    Q_OBJECT
public:
    explicit QVmbCameraclt(QObject *parent = 0);
    ~QVmbCameraclt();

    virtual bool Initiallize(int DeviceNumber,QString paramPath = "");//打开相机，并设置相机参数

    virtual bool StartView();//开始采集
    virtual bool StopView();//停止采集并且关闭相机

    virtual Mat GrabOneFrame();// 图片抓取(单次触发模式下，触发采集)

    virtual int GetCamWidth();//获取当前相机所设宽度
    virtual int GetCamHeight();//获取当前相机所设高度
    virtual int SetPara(QString featureName, QString value);//相机参数控制

    virtual int GetPara(QString featureName, CameraFeature* pCamearaFeature);

private:
    //
    // Copies the content of a byte buffer to a Qt image with respect to the image's alignment
    //
    // Parameters:
    //  [in]    pInbuffer       The byte buffer as received from the cam
    //  [in]    ePixelFormat    The pixel format of the frame
    //  [out]   OutImage        The filled Qt image
    //
    VmbErrorType CopyToImage( VmbUchar_t *pInBuffer, VmbPixelFormat_t ePixelFormat, QImage &pOutImage, const float *Matrix = NULL );


signals:

public slots:
    void OnFrameReady( int status );

private:
    AVT::VmbAPI::VimbaSystem& sys;
    AVT::VmbAPI::CameraPtr pCamera;

    VmbInt64_t camWidth;
    VmbInt64_t camHeight;

    // The current pixel format
    VmbInt64_t m_nPixelFormat;

    // Every camera has its own frame observer
    AVT::VmbAPI::IFrameObserverPtr m_pFrameObserver;

    bool bStarted;
    QImage curImg;
    QMutex mutex;
};


#endif // QVMBCAMERACLT_H
