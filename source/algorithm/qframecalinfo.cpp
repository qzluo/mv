#include "qframecalinfo.h"
#include "qsysdefine.h"
#include "QDomDocumentRW.h"

QFrameCalInfo::QFrameCalInfo()
{
    frameDir = EFRAMEDIR_DOWN;
    factorMmToPix = 1.0;
    distBtFrms = 100;
}

int QFrameCalInfo::load()
{
    QString fileName = QSysDefine::GetFrameCalInfoFileName();
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

    if (frameDir >= EFRAMEDIR_UNDEFINED) {
        frameDir = EFRAMEDIR_DOWN;
        save();
    }

    return 0;
}

int QFrameCalInfo::save()
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));

    QDomElement root = doc.createElement(QString("frameCalInfo"));
    doc.appendChild(root);

    root.appendChild(QDomDocumentRW::createDomElement(doc, "frameDir",
                                      QString("%1").arg(frameDir)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "factorMmToPix",
                                      QString("%1").arg(factorMmToPix)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "distBtFrms",
                                      QString("%1").arg(distBtFrms)));

    return QDomDocumentRW::saveToFile(doc, QSysDefine::GetFrameCalInfoFileName());
}

void QFrameCalInfo::loadElement(QDomElement &element)
{
    QString tagName = element.tagName();
    if ("frameDir" == tagName)
        frameDir = element.text().toInt();
    else if("factorMmToPix" == tagName)
        factorMmToPix = element.text().toDouble();
    else if("distBtFrms" == tagName)
        distBtFrms = element.text().toDouble();
}

double QFrameCalInfo::getDistBtFrms() const
{
    return distBtFrms;
}

void QFrameCalInfo::setDistBtFrms(double value)
{
    distBtFrms = value;
}

double QFrameCalInfo::getFactorMmToPix() const
{
    return factorMmToPix;
}

void QFrameCalInfo::setFactorMmToPix(double value)
{
    factorMmToPix = value;
}

int QFrameCalInfo::getFrameDir() const
{
    return frameDir;
}

void QFrameCalInfo::setFrameDir(int value)
{
    frameDir = value;
}
