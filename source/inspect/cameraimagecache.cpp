#include "cameraimagecache.h"
#include <QMutexLocker>

CameraImageCache* CameraImageCache::instance = new CameraImageCache();

CameraImageCache *CameraImageCache::getInstance()
{
    return instance;
}

CameraImageCache::CameraImageCache(QObject *parent) : QObject(parent)
{
    lastImage = QImage();
    frameId = 0;
    inspectFrameId = 0;

    needInspect = false;

    imageWidth = 1280;
    imageHeight = 960;
}

void CameraImageCache::startNewInspect()
{
    QMutexLocker locker(&mutex);
    needInspect = true;
}

QImage CameraImageCache::getLastImage()
{
    QMutexLocker locker(&mutex);
    return lastImage.copy();
}

unsigned int CameraImageCache::getCurFrameId()
{
    QMutexLocker locker(&mutex);
    return inspectFrameId;
}

void CameraImageCache::updateFrameId(unsigned int frameId)
{
    QMutexLocker locker(&mutex);
    this->frameId = frameId;
}

void CameraImageCache::setImageSize(int width, int height)
{
    QMutexLocker locker(&mutex);
    imageWidth = width;
    imageHeight = height;
}

void CameraImageCache::getImageSize(int *width, int *height)
{
    QMutexLocker locker(&mutex);
    *width = imageWidth;
    *height = imageHeight;
}

void CameraImageCache::onHasImage(const QImage &image)
{
    QMutexLocker locker(&mutex);
    lastImage = image.copy();
    frameId++;
    if(!needInspect)
        return;

    //改变检测状态，当检测完成后，需重新修改该状态
    needInspect = false;

    inspectFrameId = frameId;
    emit hasNewImage();
}
