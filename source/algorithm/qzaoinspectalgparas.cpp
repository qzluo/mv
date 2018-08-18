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

    return QDomDocumentRW::saveToFile(doc, QSysDefine::GetInspectAlgParasFileName());
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
