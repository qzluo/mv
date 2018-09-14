#include "projectsysinfo.h"
#include "qsysdefine.h"
#include "QDomDocumentRW.h"

ProjectSysInfo::ProjectSysInfo()
{
    cameraType = CAMERATYPE_NONE;
    portName = "Com1";
    portBaud = 9600;
    outputIsOpened = true;
    saveImageIsOpened = false;
    logOutputIsOpened = true;
    tmpFilePath = "D:\\jujubeInspectTmpFile\\";
    closingDateTime = QDateTime::currentDateTime();
}

void ProjectSysInfo::load()
{
    if (loadFromFile(QSysDefine::GetSettingSysFileName()) < 0)
        save();
}

int ProjectSysInfo::save()
{
    return saveToFile(QSysDefine::GetSettingSysFileName());
}

int ProjectSysInfo::loadFromFile(QString &fileName)
{
    QDomDocument doc;
    QFile file(fileName);

    if (!file.exists())
        return -1;

    if (!file.open(QIODevice::ReadOnly))
        return -1;

    if (!doc.setContent(&file)) {
        file.close();
        return -1;
    }

    file.close();

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() != "SysInfo")
        return -1;

    QDomNode n = docElem.firstChild();
    while (!n.isNull()) {
        if (n.isElement()) {
            QDomElement element = n.toElement();
            loadElement(element);
        }

        n = n.nextSibling();
    }

    return 0;
}

int ProjectSysInfo::saveToFile(QString &fileName)
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));

    QDomElement root = doc.createElement(QString("SysInfo"));
    doc.appendChild(root);

    root.appendChild(QDomDocumentRW::createDomElement(doc, "camType",
                                      QString("%1").arg(cameraType)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "portName",
                                      QString("%1").arg(portName)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "portBaud",
                                      QString("%1").arg(portBaud)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "outputIsOpened",
                                      QString("%1").arg(outputIsOpened ? 1 : 0)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "saveImageIsOpened",
                                      QString("%1").arg(saveImageIsOpened ? 1 : 0)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "logOutputIsOpened",
                                      QString("%1").arg(logOutputIsOpened ? 1 : 0)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "tmpFilePath",
                                      QString("%1").arg(tmpFilePath)));

    if (closingDateTime.isValid())
        root.appendChild(QDomDocumentRW::createDomElement(doc, "ClosingDate",
            closingDateTime.toString("yyyy-MM-dd HH:mm:ss")));

    return QDomDocumentRW::saveToFile(doc, fileName);
}

void ProjectSysInfo::loadElement(QDomElement &element)
{
    QString tagName = element.tagName();

    if ("camType" == tagName)
        cameraType = element.text().toInt();
    else if("portName" == tagName)
        portName = element.text();
    else if("portBaud" == tagName)
        portBaud = element.text().toInt();
    else if("outputIsOpened" == tagName)
        outputIsOpened = element.text().toInt() ? true : false;
    else if("saveImageIsOpened" == tagName)
        saveImageIsOpened = element.text().toInt() ? true : false;
    else if("logOutputIsOpened" == tagName)
        logOutputIsOpened = element.text().toInt() ? true : false;
    else if("tmpFilePath" == tagName)
        tmpFilePath = element.text();
    else if("ClosingDate" == tagName)
        closingDateTime = QDateTime::fromString(element.text(),
                                                "yyyy-MM-dd HH:mm:ss");
}

QString ProjectSysInfo::getTmpFilePath() const
{
    return tmpFilePath;
}

void ProjectSysInfo::setTmpFilePath(const QString &value)
{
    tmpFilePath = value;
}

bool ProjectSysInfo::getLogOutputIsOpened() const
{
    return logOutputIsOpened;
}

void ProjectSysInfo::setLogOutputIsOpened(bool value)
{
    logOutputIsOpened = value;
}

bool ProjectSysInfo::getSaveImageIsOpened() const
{
    return saveImageIsOpened;
}

void ProjectSysInfo::setSaveImageIsOpened(bool value)
{
    saveImageIsOpened = value;
}

bool ProjectSysInfo::getOutputIsOpened() const
{
    return outputIsOpened;
}

void ProjectSysInfo::setOutputIsOpened(bool value)
{
    outputIsOpened = value;
}

int ProjectSysInfo::getPortBaud() const
{
    return portBaud;
}

void ProjectSysInfo::setPortBaud(int value)
{
    portBaud = value;
}

QString ProjectSysInfo::getPortName() const
{
    return portName;
}

void ProjectSysInfo::setPortName(const QString &value)
{
    portName = value;
}

int ProjectSysInfo::getCameraType() const
{
    return cameraType;
}

void ProjectSysInfo::setCameraType(int value)
{
    cameraType = value;
}
