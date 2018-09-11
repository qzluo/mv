#include "qsysdefine.h"

#include <QDir>
#include <QString>

QSysDefine::QSysDefine()
{

}

QString QSysDefine::GetSettingSysFileName()
{
    QDir dir;
    return dir.currentPath() + "//jujubeInspectPrjSysInfo.xml";
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

QString getJujubeDescFromClass(int classId)
{
    QString strRet = "";
    switch (classId) {
    case ZAO_CLASS_GOOD1:
        strRet = QObject::tr("high-class");
        break;

    case ZAO_CLASS_GOOD2:
        strRet = QObject::tr("first-class");
        break;

    case ZAO_CLASS_GOOD3:
        strRet = QObject::tr("second-class");
        break;

    case ZAO_CLASS_GOOD4:
        strRet = QObject::tr("third-class");
        break;

    case ZAO_CLASS_BAD1:
        strRet = QObject::tr("pipi");
        break;

    case ZAO_CLASS_BAD2:
        strRet = QObject::tr("split jujube");
        break;

    case ZAO_CLASS_BAD3:
        strRet = QObject::tr("excoriated jujube");
        break;

    case ZAO_CLASS_BAD4:
        strRet = QObject::tr("black jujube");
        break;

    default:
        strRet = QObject::tr("None");
        break;
    }

    return strRet;
}
