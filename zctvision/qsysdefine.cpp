#include "qsysdefine.h"

#include <QDir>

QSysDefine::QSysDefine()
{

}

QString QSysDefine::GetSettingSysFileName()
{
    QDir dir;
    return dir.currentPath() + "//zaoInspectPrjSysInfo.xml";
}

QString QSysDefine::GetFrameCalInfoFileName()
{
    QDir dir;
    return dir.currentPath() + "//frameCalInfo.xml";
}

QString QSysDefine::GetInspectAlgParasFileName()
{
    QDir dir;
    return dir.currentPath() + "//algParameter.xml";
}

QString QSysDefine::GetRecognizeModelFilePath()
{
    QDir dir;
    return dir.currentPath();
}

QString QSysDefine::GetRecognizeModelFileName()
{
    return QString("model");
}
