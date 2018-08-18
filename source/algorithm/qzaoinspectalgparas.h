#ifndef QZAOINSPECTALGPARAS_H
#define QZAOINSPECTALGPARAS_H

#include <QtXml>

class QZaoInspectAlgParas
{
public:
    QZaoInspectAlgParas();

    int load(void);
    int save(void);

    double getClass_good1_length() const;
    void setClass_good1_length(double value);

    double getClass_good1_width() const;
    void setClass_good1_width(double value);

    double getClass_good2_length() const;
    void setClass_good2_length(double value);

    double getClass_good2_width() const;
    void setClass_good2_width(double value);

    double getClass_good3_length() const;
    void setClass_good3_length(double value);

    double getClass_good3_width() const;
    void setClass_good3_width(double value);

private:
    void loadElement(QDomElement& element);

private:
    double class_good1_length;
    double class_good1_width;
    double class_good2_length;
    double class_good2_width;
    double class_good3_length;
    double class_good3_width;
};

#endif // QZAOINSPECTALGPARAS_H
