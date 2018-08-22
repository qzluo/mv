#include "algorithmmanager.h"
#include <QDebug>

AlgorithmManager::AlgorithmManager(QObject *parent) : QObject(parent)
{

}

int AlgorithmManager::init()
{
    return zaoInspectAlg.init();
}

int AlgorithmManager::resetImageSize(int imgWidth, int imgHeight)
{
    return zaoInspectAlg.resetImageSize(imgWidth, imgHeight);
}

int AlgorithmManager::reset()
{
    return zaoInspectAlg.reset();
}

int AlgorithmManager::inspect(const QImage &cameraImg, QImage &outImg)
{
    qDebug() << "AlgorithmManager::inspect";
    return zaoInspectAlg.inspect(cameraImg, outImg);
}

int AlgorithmManager::getDataVariant(const QString &dataName, QVariant &var)
{
    return zaoInspectAlg.getDataVariant(dataName, var);
}

QFrameCalInfo *AlgorithmManager::getPFrameCalInfo()
{
    return zaoInspectAlg.getPFrameCalInfo();
}

QZaoInspectAlgParas *AlgorithmManager::getPInspectAlgParas()
{
    return zaoInspectAlg.getPInspectAlgParas();
}
