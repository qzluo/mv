#include "cameraimagecache.h"
#include <QMutexLocker>

CameraImageCache* CameraImageCache::instance = new CameraImageCache();

CameraImageCache *CameraImageCache::getInstance()
{
    return instance;
}

CameraImageCache::CameraImageCache(QObject *parent) : QObject(parent)
{
    cacheMaxNum = 0;
    imageList.clear();
    lastImage = QImage();
    frameId = 0;
    inspectFrameId = 0;

    imageWidth = 1280;
    imageHeight = 960;
}

void CameraImageCache::startNewCache(int cacheNum)
{
    QMutexLocker locker(&mutex);
    cacheMaxNum = cacheNum;
    imageList.clear();
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
    frameId = frameId;
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
    if(imageList.size() >= cacheMaxNum)
        return;

    imageList.push_back(image.copy());
    locker.unlock();

    if(imageList.size() == cacheMaxNum) {
        inspectFrameId = frameId;
        emit cacheFull();
    }
}
