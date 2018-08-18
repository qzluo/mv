#include "qinspectctl.h"
#include "cameraimagecache.h"
#include <QDebug>
#include <FileLogger.h>

QInspectWorker::QInspectWorker(QObject *parent) : QObject(parent)
{
    pAlgorithmManager = NULL;
}

void QInspectWorker::onHasImage(QImage image)
{
    if (!pAlgorithmManager)
        return ;

    pAlgorithmManager->inspect(image);

    emit inspectDone();
}

void QInspectWorker::onImageCacheFull()
{
    QImage image = cic->getLastImage();
    onHasImage(image);
}

AlgorithmManager *QInspectWorker::getAlgorithmManager() const
{
    return pAlgorithmManager;
}

void QInspectWorker::setAlgorithmManager(AlgorithmManager *value)
{
    pAlgorithmManager = value;
}

QInspectCtl::QInspectCtl(QObject *parent) : QObject(parent)
{
    inspectWorker = NULL;
}

QInspectCtl::~QInspectCtl()
{
    inspectThread.quit();
    inspectThread.wait();
}

int QInspectCtl::initThread()
{
    inspectWorker = new QInspectWorker;
    inspectWorker->moveToThread(&inspectThread);

    connect(&inspectThread, &QThread::finished, inspectWorker, &QObject::deleteLater);
    connect(inspectWorker, &QInspectWorker::inspectDone,
            this, &QInspectCtl::inspectDone);

    //相机
    connect(cic, &CameraImageCache::cacheFull,
            inspectWorker, &QInspectWorker::onImageCacheFull);

    inspectThread.start();

    return 0;
}

void QInspectCtl::setAlgorithmManager(AlgorithmManager *value)
{
    if (inspectWorker)
        inspectWorker->setAlgorithmManager(value);
}

