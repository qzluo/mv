#ifndef QDOMDOCUMENTRW_H
#define QDOMDOCUMENTRW_H


#include <QString>
#include <QtXml>

class QDomDocumentRW
{
public:
    QDomDocumentRW();
    ~QDomDocumentRW();

    static QDomElement createDomElement(QDomDocument &doc, QString tagName,
                                        QString text = "", QString id = "");
    static int saveToFile(const QDomDocument &doc, QString szFileName);
};

#endif // QDOMDOCUMENTRW_H
