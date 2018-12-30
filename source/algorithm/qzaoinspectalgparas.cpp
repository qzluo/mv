#include "qzaoinspectalgparas.h"

#include "qsysdefine.h"
#include "QDomDocumentRW.h"

#include <QJsonDocument>
#include <QJsonObject>

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

void QZaoInspectAlgParas::load()
{
    if (loadFromFile(QSysDefine::GetInspectAlgParasFileName()) < 0)
        save();
}

int QZaoInspectAlgParas::save()
{
    return saveToFile(QSysDefine::GetInspectAlgParasFileName());
}

int QZaoInspectAlgParas::loadFromFile(QString &fileName)
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
    if (docElem.tagName() != "AlgParameters")
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

int QZaoInspectAlgParas::saveToFile(QString &fileName)
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

    return QDomDocumentRW::saveToFile(doc, fileName);
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

void QZaoInspectAlgParas::updateParasWithIncrement(const QByteArray &incrementDatagram)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(incrementDatagram, &json_error);
    if (json_error.error != QJsonParseError::NoError)
        return;

    if (!parse_doucment.isObject())
        return;

    QJsonObject obj = parse_doucment.object();
    QJsonValue json_value;

    if (obj.contains("class_good1_length")) {
        json_value = obj.take("class_good1_length");
        if (json_value.isDouble()) {
            class_good1_length += json_value.toDouble();
            if (class_good1_length < 0)
                class_good1_length = 0;
        }
    }

    if (obj.contains("class_good1_width")) {
        json_value = obj.take("class_good1_width");
        if (json_value.isDouble()) {
            class_good1_width += json_value.toDouble();
            if (class_good1_width < 0)
                class_good1_width = 0;
        }
    }

    if (obj.contains("class_good2_length")) {
        json_value = obj.take("class_good2_length");
        if (json_value.isDouble()) {
            class_good2_length += json_value.toDouble();
            if (class_good2_length < 0)
                class_good2_length = 0;
        }
    }

    if (obj.contains("class_good2_width")) {
        json_value = obj.take("class_good2_width");
        if (json_value.isDouble()) {
            class_good2_width += json_value.toDouble();
            if (class_good2_width < 0)
                class_good2_width = 0;
        }
    }

    if (obj.contains("class_good3_length")) {
        json_value = obj.take("class_good3_length");
        if (json_value.isDouble()) {
            class_good3_length += json_value.toDouble();
            if (class_good3_length < 0)
                class_good3_length = 0;
        }
    }

    if (obj.contains("class_good3_width")) {
        json_value = obj.take("class_good3_width");
        if (json_value.isDouble()) {
            class_good3_width += json_value.toDouble();
            if (class_good3_width < 0)
                class_good3_width = 0;
        }
    }

    if (obj.contains("Imin")) {
        json_value = obj.take("Imin");
        if (json_value.isDouble()) {
            int old_value = Imin + int(json_value.toDouble());
            if (old_value < 0)
                old_value = 0;
            else if (old_value > 255)
                old_value = 255;

            Imin = old_value;
        }
    }

    if (obj.contains("Bmin")) {
        json_value = obj.take("Bmin");
        if (json_value.isDouble()) {
            int old_value = Bmin + int(json_value.toDouble());
            if (old_value < 0)
                old_value = 0;
            else if (old_value > 255)
                old_value = 255;

            Bmin = old_value;
        }
    }

    if (obj.contains("Rmin")) {
        json_value = obj.take("Rmin");
        if (json_value.isDouble()) {
            int old_value = Rmin + int(json_value.toDouble());
            if (old_value < 0)
                old_value = 0;
            else if (old_value > 255)
                old_value = 255;

            Rmin = old_value;
        }
    }

    if (obj.contains("Imax")) {
        json_value = obj.take("Imax");
        if (json_value.isDouble()) {
            int old_value = Imax + int(json_value.toDouble());
            if (old_value < 0)
                old_value = 0;
            else if (old_value > 255)
                old_value = 255;

            Imax = old_value;
        }
    }

    if (obj.contains("Bmax")) {
        json_value = obj.take("Bmax");
        if (json_value.isDouble()) {
            int old_value = Bmax + int(json_value.toDouble());
            if (old_value < 0)
                old_value = 0;
            else if (old_value > 255)
                old_value = 255;

            Bmax = old_value;
        }
    }

    if (obj.contains("Rmax")) {
        json_value = obj.take("Rmax");
        if (json_value.isDouble()) {
            int old_value = Rmax + int(json_value.toDouble());
            if (old_value < 0)
                old_value = 0;
            else if (old_value > 255)
                old_value = 255;

            Rmax = old_value;
        }
    }

    if (obj.contains("Wmin")) {
        json_value = obj.take("Wmin");
        if (json_value.isDouble()) {
            Wmin += int(json_value.toDouble());
            if (Wmin < 0)
                Wmin = 0;
        }
    }

    if (obj.contains("Hmin")) {
        json_value = obj.take("Hmin");
        if (json_value.isDouble()) {
            Hmin += int(json_value.toDouble());
            if (Hmin < 0)
                Hmin = 0;
        }
    }

    if (obj.contains("RatioMax")) {
        json_value = obj.take("RatioMax");
        if (json_value.isDouble()) {
            RatioMax += float(json_value.toDouble());
            if (RatioMax < 0)
                RatioMax = 0;
        }
    }

    save();
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
