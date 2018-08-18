#include "projectsysinfo.h"
#include "qsysdefine.h"
#include "QDomDocumentRW.h"

ProjectSysInfo::ProjectSysInfo()
{
    cameraType = CAMERATYPE_AVT;
    serialPortNo = -1;
    closingDateTime = QDateTime::currentDateTime();
}

int ProjectSysInfo::load()
{
    QString fileName = QSysDefine::GetSettingSysFileName();
    QDomDocument doc;
    QFile file(fileName);

    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly))
            return -1;

        if (!doc.setContent(&file)) {
            file.close();
            return -1;
        }

        file.close();

        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();
        while (!n.isNull()) {
            if (n.isElement()) {
                QDomElement element = n.toElement();
                loadElement(element);
            }

            n = n.nextSibling();
        }
    }
    else
        save();

    return 0;
}

int ProjectSysInfo::save()
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));

    QDomElement root = doc.createElement(QString("SysInfo"));
    doc.appendChild(root);

    root.appendChild(QDomDocumentRW::createDomElement(doc, "camType",
                                      QString("%1").arg(cameraType)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "serioalPort",
                                      QString("%1").arg(serialPortNo)));
    if (closingDateTime.isValid())
        root.appendChild(QDomDocumentRW::createDomElement(doc, "ClosingDate",
            closingDateTime.toString("yyyy-MM-dd HH:mm:ss")));

    return QDomDocumentRW::saveToFile(doc, QSysDefine::GetSettingSysFileName());
}

void ProjectSysInfo::loadElement(QDomElement &element)
{
    QString tagName = element.tagName();

    if ("camType" == tagName)
        cameraType = element.text().toInt();
    else if("serioalPort" == tagName)
        serialPortNo = element.text().toInt();
    else if("ClosingDate" == tagName)
        closingDateTime = QDateTime::fromString(element.text(),
                                                "yyyy-MM-dd HH:mm:ss");
}

int ProjectSysInfo::getSerialPortNo() const
{
    return serialPortNo;
}

void ProjectSysInfo::setSerialPortNo(int value)
{
    serialPortNo = value;
}

int ProjectSysInfo::getCameraType() const
{
    return cameraType;
}

void ProjectSysInfo::setCameraType(int value)
{
    cameraType = value;
}
