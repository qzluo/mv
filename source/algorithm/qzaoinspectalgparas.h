#ifndef QZAOINSPECTALGPARAS_H
#define QZAOINSPECTALGPARAS_H

#include <QtXml>
#include "HQEDetector.h"

class QZaoInspectAlgParas
{
public:
    QZaoInspectAlgParas();

    void load(void);
    int save(void);

    int loadFromFile(QString& fileName);
    int saveToFile(QString& fileName);

    HQEDetectorParams cerateDetectParas(void);

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

    uchar getImin() const;
    void setImin(const uchar &value);

    uchar getBmin() const;
    void setBmin(const uchar &value);

    uchar getRmin() const;
    void setRmin(const uchar &value);

    uchar getImax() const;
    void setImax(const uchar &value);

    uchar getBmax() const;
    void setBmax(const uchar &value);

    uchar getRmax() const;
    void setRmax(const uchar &value);

    int getWmin() const;
    void setWmin(int value);

    int getHmin() const;
    void setHmin(int value);

    float getRatioMax() const;
    void setRatioMax(float value);

    void updateParasWithIncrement(const QByteArray& incrementDatagram);

private:
    void loadElement(QDomElement& element);

private:
    double class_good1_length;
    double class_good1_width;
    double class_good2_length;
    double class_good2_width;
    double class_good3_length;
    double class_good3_width;

    uchar Imin;
    uchar Bmin;
    uchar Rmin;
    uchar Imax;
    uchar Bmax;
    uchar Rmax;
    int   Wmin;//min width of an object
    int   Hmin;//min height of an object;
    float RatioMax;//max long_edge/short_edge
};

#endif // QZAOINSPECTALGPARAS_H
