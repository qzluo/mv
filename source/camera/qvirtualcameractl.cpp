#include "qvirtualcameractl.h"

#include <QMutexLocker>
#include <QTimer>
#include <QDir>
#include <QDebug>

#define TEMP_CAM_VIR "F:/VirCam/TmpImage%d/"

QVirtualCameraWorker::QVirtualCameraWorker(QObject *parent) : QObject(parent)
{
    bStarted = false;
    capturePeriod = 2000;
    filePath = "";
    curImgIndex = -1;
}

QVirtualCameraWorker::~QVirtualCameraWorker()
{

}

void QVirtualCameraWorker::onStartCapture()
{
    bStarted = true;
    curImgIndex = -1;
    updateCapture();
}

void QVirtualCameraWorker::onStopCapture()
{
    QMutexLocker locker(&mutex);
    bStarted = false;
}

void QVirtualCameraWorker::updateCapture()
{
    QMutexLocker locker(&mutex);

    if (!bStarted)
        return ;

    if (imgFileNames.size() == 0) {
        qDebug() << "image count is 0.";
        bStarted = false;
        return ;
    }

    // get a new image
    curImgIndex++;

    if (curImgIndex >= imgFileNames.size())
        curImgIndex = 0;

    QString imgpath = filePath + imgFileNames.at(curImgIndex);
    QImage img(imgpath);

    emit hasImage(img);

    //next capture
    QTimer::singleShot(capturePeriod, this, SLOT(updateCapture()));
}

QStringList QVirtualCameraWorker::getImgFileNames() const
{
    return imgFileNames;
}

void QVirtualCameraWorker::setImgFileNames(const QStringList &value)
{
    imgFileNames = value;
}

QString QVirtualCameraWorker::getFilePath() const
{
    return filePath;
}

void QVirtualCameraWorker::setFilePath(const QString &value)
{
    filePath = value;
}


////
/// \brief QVirtualCameraCtl::QVirtualCameraCtl
/// \param parent
///
QVirtualCameraCtl::QVirtualCameraCtl(QObject *parent) : CameraCtl(parent)
{
    camWidth = 0;
    camHeight = 0;

    worker = new QVirtualCameraWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    connect(this, &QVirtualCameraCtl::startCapture,
            worker, &QVirtualCameraWorker::onStartCapture);

    connect(this, &QVirtualCameraCtl::stopCapture,
            worker, &QVirtualCameraWorker::onStopCapture);

    workerThread.start();
}

QVirtualCameraCtl::~QVirtualCameraCtl()
{
    workerThread.quit();
    workerThread.wait();
}

bool QVirtualCameraCtl::Initiallize(int DeviceNumber, QString paramPath)
{
    QString filePath;
    filePath.sprintf(TEMP_CAM_VIR, DeviceNumber + 1);

    QDir dir(filePath);
    bool bExists = dir.exists();
    if (bExists) {
        worker->setFilePath(filePath);

        QStringList filters;
        filters << "*.bmp" << "*.jpg" << "*.png";
        dir.setNameFilters(filters);

        QStringList fileNames = dir.entryList();
        worker->setImgFileNames(fileNames);

        if(!fileNames.empty()) {
            QString imgpath = filePath + fileNames.at(0);
            QImage img(imgpath);
            camWidth = img.width();
            camHeight = img.height();
        }
        else {
            camWidth = 0;
            camHeight = 0;
            return false;
        }
    }

    return bExists;
}

bool QVirtualCameraCtl::StartView()
{
    connect(worker, &QVirtualCameraWorker::hasImage,
            this, &QVirtualCameraCtl::hasImage);

    emit startCapture();
    return true;
}

bool QVirtualCameraCtl::StopView()
{
    disconnect(worker, &QVirtualCameraWorker::hasImage,
               this, &QVirtualCameraCtl::hasImage);

    emit stopCapture();
    return true;
}

Mat QVirtualCameraCtl::GrabOneFrame()
{
    Mat image;
    return image;
}

int QVirtualCameraCtl::GetCamWidth()
{
    return camWidth;
}

int QVirtualCameraCtl::GetCamHeight()
{
    return camHeight;
}

void QVirtualCameraCtl::SetPara()
{

}

