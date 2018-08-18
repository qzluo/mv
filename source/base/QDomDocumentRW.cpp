#include "QDomDocumentRW.h"

QDomDocumentRW::QDomDocumentRW()
{

}

QDomDocumentRW::~QDomDocumentRW()
{

}

QDomElement QDomDocumentRW::createDomElement(QDomDocument& doc,
                                             QString tagName,
                                             QString text,
                                             QString id)
{
    QDomElement element = doc.createElement(tagName);

    if ("" != id) {
        QDomAttr idAttr = doc.createAttribute(QString("id"));
        idAttr.setValue(id);
        element.setAttributeNode(idAttr);
    }

    if ("" != text) {
        QDomText Domtext = doc.createTextNode(text);;
        element.appendChild(Domtext);
    }

    return element;
}

int QDomDocumentRW::saveToFile(const QDomDocument& doc, QString szFileName)
{
    QFile file(szFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug()<<"openfile false";
        return -1;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    return 0;
}
