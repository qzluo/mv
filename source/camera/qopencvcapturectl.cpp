#include "qopencvcapturectl.h"
#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QMutexLocker>
#include <QFile>

#include "Mat2QImage.h"
#include "QDShowFuns.h"

///
/// \brief QOpenCVCaptureWorker::QOpenCVCaptureWorker
/// \param parent
///
QOpenCVCaptureWorker::QOpenCVCaptureWorker(QObject *parent) : QObject(parent)
{
    bStartd = false;
    capturePeriod = 50;
}

QOpenCVCaptureWorker::~QOpenCVCaptureWorker()
{
    QMutexLocker locker(&mutex);

    if (cap.isOpened())
        cap.release();
}

bool QOpenCVCaptureWorker::Initiallize(int device, QString paramPath)
{
    int cameraCount = QDShowFuns::getCameraCount();
    qDebug() << QString("camera count: %1").arg(cameraCount);

    int deviceNumber = getCameraDeviceNumber(device);
    if (deviceNumber < 0)
        return false;

    int width = 0;
    int height = 0;
    int ret = QDShowFuns::getDeviceMaxResolusion(deviceNumber, width, height);
    if (ret < 0)
        return false;

    QMutexLocker locker(&mutex);
    if (cap.isOpened())
        cap.release();

    cap.open(deviceNumber);
    if (!cap.isOpened())
        return false;

    //width, height
    if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, width))
        return false;

    if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, height))
        return false;

    return true;
}

cv::Mat QOpenCVCaptureWorker::GrabOneFrame()
{
    QMutexLocker locker(&mutex);

    cv::Mat frame;
    for (int i = 0; i < 2; ++i) {
        if (!cap.grab() || !cap.retrieve(frame)) {
            qDebug() << "grab picture failed!";
            return cv::Mat();
        }
    }

    return frame;
}

int QOpenCVCaptureWorker::getCamWidth()
{
    QMutexLocker locker(&mutex);

    if (!cap.isOpened())
        return 0;

    return int(cap.get(CV_CAP_PROP_FRAME_WIDTH));
}

int QOpenCVCaptureWorker::getCamHeight()
{
    QMutexLocker locker(&mutex);

    if (!cap.isOpened())
        return 0;

    return int(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
}

void QOpenCVCaptureWorker::showPropertyPage(int nCamID)
{
    QDShowFuns::showPropertyPage(nCamID);
}

int QOpenCVCaptureWorker::getCameraDeviceNumber(int index)
{
    int cameraCount = QDShowFuns::getCameraCount();
    if (index >= cameraCount)
        return -1;

    QStringList camNames;
    char szCamName[1024] = {};
    for (int i = 0; i < cameraCount; ++i) {
        QDShowFuns::getCameraName(i, szCamName, sizeof(szCamName));
        camNames.push_back(QString("%1").arg(szCamName));
    }

    //排序后的产品名称列表
    QStringList sortedCamNames = camNames;
    qSort(sortedCamNames.begin(), sortedCamNames.end());

    //排序后的产品名称索引在默认列表的位置
    QStringList::const_iterator findItor = qFind(camNames, sortedCamNames[index]);
    int deviceNumber = findItor - camNames.begin();

    Q_ASSERT(deviceNumber < cameraCount);

    return deviceNumber;
}

void QOpenCVCaptureWorker::onStartCapture()
{
    bStartd = true;

    updateCapture();
}

void QOpenCVCaptureWorker::onStopCapture()
{
    QMutexLocker locker(&mutex);
    bStartd = false;
}

void QOpenCVCaptureWorker::updateCapture()
{
    QMutexLocker locker(&mutex);

    if (!bStartd)
        return ;

    // get a new frame from camera
    cv::Mat frame;
    if (!cap.grab() || !cap.retrieve(frame)) {
        qDebug() << "grab picture failed!";
        return ;
    }

    // emit signal
    QImage img = ImageTransform::MatToQImage(frame);
    emit hasImage(img);

    //next capture
    QTimer::singleShot(capturePeriod, this, SLOT(updateCapture()));
}

////
/// \brief QOpenCVCaptureCtl::QOpenCVCaptureCtl
/// \param parent
///
QOpenCVCaptureCtl::QOpenCVCaptureCtl(QObject *parent) : CameraCtl(parent)
{
    worker = new QOpenCVCaptureWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    connect(this, &QOpenCVCaptureCtl::startCapture,
            worker, &QOpenCVCaptureWorker::onStartCapture);

    connect(this, &QOpenCVCaptureCtl::stopCapture,
            worker, &QOpenCVCaptureWorker::onStopCapture);

    workerThread.start();
}

QOpenCVCaptureCtl::~QOpenCVCaptureCtl()
{
    workerThread.quit();
    workerThread.wait();
}

bool QOpenCVCaptureCtl::Initiallize(int DeviceNumber, QString paramPath)
{
    return worker->Initiallize(DeviceNumber, paramPath);
}

bool QOpenCVCaptureCtl::StartView()
{
    connect(worker, &QOpenCVCaptureWorker::hasImage,
            this, &QOpenCVCaptureCtl::hasImage);

    emit startCapture();
    return true;
}

bool QOpenCVCaptureCtl::StopView()
{
    disconnect(worker, &QOpenCVCaptureWorker::hasImage,
               this, &QOpenCVCaptureCtl::hasImage);

    emit stopCapture();
    return true;
}

cv::Mat QOpenCVCaptureCtl::GrabOneFrame()
{
    return worker->GrabOneFrame();
}

int QOpenCVCaptureCtl::GetCamWidth()
{
    return worker->getCamWidth();
}

int QOpenCVCaptureCtl::GetCamHeight()
{
    return worker->getCamHeight();
}

int QOpenCVCaptureCtl::SetPara(QString featureName, QString value)
{
    return 0;
}

int QOpenCVCaptureCtl::GetPara(QString featureName, CameraFeature *pCamearaFeature)
{
    return 0;
}

void QOpenCVCaptureCtl::showPropertyPage(int nCamID)
{
    worker->showPropertyPage(nCamID);
}
