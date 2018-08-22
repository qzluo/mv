#include "qzaoinspectalgparas.h"

#include "qsysdefine.h"
#include "QDomDocumentRW.h"

QZaoInspectAlgParas::QZaoInspectAlgParas()
{
    class_good1_length = 10;
    class_good1_width = 5;
    class_good2_length = 8;
    class_good2_width = 4;
    class_good3_length = 6;
    class_good3_width = 3;

    Imin=50;
    Bmin=55;
    Rmin=140;

    Imax=200;
    Bmax=130;
    Rmax=235;
    Wmin=50;
    Hmin=50;

    RatioMax=3.0f;
}

int QZaoInspectAlgParas::load()
{
    QString fileName = QSysDefine::GetInspectAlgParasFileName();
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

int QZaoInspectAlgParas::save()
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));

    QDomElement root = doc.createElement(QString("AlgParameters"));
    doc.appendChild(root);

    root.appendChild(QDomDocumentRW::createDomElement(doc, "class_good1_length",
                                      QString("%1").arg(class_good1_length)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "class_good1_width",
                                      QString("%1").arg(class_good1_width)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "class_good2_length",
                                      QString("%1").arg(class_good2_length)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "class_good2_width",
                                      QString("%1").arg(class_good2_width)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "class_good3_length",
                                      QString("%1").arg(class_good3_length)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "class_good3_width",
                                      QString("%1").arg(class_good3_width)));

    root.appendChild(QDomDocumentRW::createDomElement(doc, "Imin",
                                      QString("%1").arg(Imin)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Imax",
                                      QString("%1").arg(Imax)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Bmin",
                                      QString("%1").arg(Bmin)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Bmax",
                                      QString("%1").arg(Bmax)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Rmin",
                                      QString("%1").arg(Rmin)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Rmax",
                                      QString("%1").arg(Rmax)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Wmin",
                                      QString("%1").arg(Wmin)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "Hmin",
                                      QString("%1").arg(Hmin)));
    root.appendChild(QDomDocumentRW::createDomElement(doc, "RatioMax",
                                      QString("%1").arg(RatioMax)));

    return QDomDocumentRW::saveToFile(doc, QSysDefine::GetInspectAlgParasFileName());
}

HQEDetectorParams QZaoInspectAlgParas::cerateDetectParas()
{
    HQEDetectorParams detectParas = {};
    detectParas.Imin = Imin;
    detectParas.Bmin = Bmin;
    detectParas.Rmin = Rmin;
    detectParas.Imax = Imax;
    detectParas.Bmax = Bmax;
    detectParas.Rmax = Rmax;
    detectParas.Wmin = Wmin;
    detectParas.Hmin = Hmin;
    detectParas.RatioMax = RatioMax;

    return detectParas;
}

void QZaoInspectAlgParas::loadElement(QDomElement &element)
{
    QString tagName = element.tagName();
    if ("class_good1_length" == tagName)
        class_good1_length = element.text().toDouble();
    else if("class_good1_width" == tagName)
        class_good1_width = element.text().toDouble();
    else if ("class_good2_length" == tagName)
        class_good2_length = element.text().toDouble();
    else if("class_good2_width" == tagName)
        class_good2_width = element.text().toDouble();
    else if ("class_good3_length" == tagName)
        class_good3_length = element.text().toDouble();
    else if("class_good3_width" == tagName)
        class_good3_width = element.text().toDouble();
    else if("Imin" == tagName)
        Imin = (element.text().toUInt()) & 0xFF;
    else if("Imax" == tagName)
        Imax = (element.text().toUInt()) & 0xFF;
    else if("Bmin" == tagName)
        Bmin = (element.text().toUInt()) & 0xFF;
    else if("Bmax" == tagName)
        Bmax = (element.text().toUInt()) & 0xFF;
    else if("Rmin" == tagName)
        Rmin = (element.text().toUInt()) & 0xFF;
    else if("Rmax" == tagName)
        Rmax = (element.text().toUInt()) & 0xFF;
    else if("Wmin" == tagName)
        Wmin = element.text().toInt();
    else if("Hmin" == tagName)
        Hmin = element.text().toInt();
    else if("RatioMax" == tagName)
        RatioMax = element.text().toFloat();
}

float QZaoInspectAlgParas::getRatioMax() const
{
    return RatioMax;
}

void QZaoInspectAlgParas::setRatioMax(float value)
{
    RatioMax = value;
}

int QZaoInspectAlgParas::getHmin() const
{
    return Hmin;
}

void QZaoInspectAlgParas::setHmin(int value)
{
    Hmin = value;
}

int QZaoInspectAlgParas::getWmin() const
{
    return Wmin;
}

void QZaoInspectAlgParas::setWmin(int value)
{
    Wmin = value;
}

uchar QZaoInspectAlgParas::getRmax() const
{
    return Rmax;
}

void QZaoInspectAlgParas::setRmax(const uchar &value)
{
    Rmax = value;
}

uchar QZaoInspectAlgParas::getBmax() const
{
    return Bmax;
}

void QZaoInspectAlgParas::setBmax(const uchar &value)
{
    Bmax = value;
}

uchar QZaoInspectAlgParas::getImax() const
{
    return Imax;
}

void QZaoInspectAlgParas::setImax(const uchar &value)
{
    Imax = value;
}

uchar QZaoInspectAlgParas::getRmin() const
{
    return Rmin;
}

void QZaoInspectAlgParas::setRmin(const uchar &value)
{
    Rmin = value;
}

uchar QZaoInspectAlgParas::getBmin() const
{
    return Bmin;
}

void QZaoInspectAlgParas::setBmin(const uchar &value)
{
    Bmin = value;
}

uchar QZaoInspectAlgParas::getImin() const
{
    return Imin;
}

void QZaoInspectAlgParas::setImin(const uchar &value)
{
    Imin = value;
}

double QZaoInspectAlgParas::getClass_good3_width() const
{
    return class_good3_width;
}

void QZaoInspectAlgParas::setClass_good3_width(double value)
{
    class_good3_width = value;
}

double QZaoInspectAlgParas::getClass_good3_length() const
{
    return class_good3_length;
}

void QZaoInspectAlgParas::setClass_good3_length(double value)
{
    class_good3_length = value;
}

double QZaoInspectAlgParas::getClass_good2_width() const
{
    return class_good2_width;
}

void QZaoInspectAlgParas::setClass_good2_width(double value)
{
    class_good2_width = value;
}

double QZaoInspectAlgParas::getClass_good2_length() const
{
    return class_good2_length;
}

void QZaoInspectAlgParas::setClass_good2_length(double value)
{
    class_good2_length = value;
}

double QZaoInspectAlgParas::getClass_good1_width() const
{
    return class_good1_width;
}

void QZaoInspectAlgParas::setClass_good1_width(double value)
{
    class_good1_width = value;
}

double QZaoInspectAlgParas::getClass_good1_length() const
{
    return class_good1_length;
}

void QZaoInspectAlgParas::setClass_good1_length(double value)
{
    class_good1_length = value;
}
