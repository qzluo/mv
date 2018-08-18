#ifndef QFRAMECALINFO_H
#define QFRAMECALINFO_H

#include <QtXml>

class QFrameCalInfo
{
public:
    QFrameCalInfo();

    enum EFrameDir {
        EFRAMEDIR_DOWN = 0,
        EFRAMEDIR_RIGHT,
        EFRAMEDIR_UP,
        EFRAMEDIR_LEFT,
        EFRAMEDIR_UNDEFINED,
    };

    int load(void);
    int save(void);

    int getFrameDir() const;
    void setFrameDir(int value);

    double getFactorMmToPix() const;
    void setFactorMmToPix(double value);

    double getDistBtFrms() const;
    void setDistBtFrms(double value);

private:
    void loadElement(QDomElement& element);

private:
    int frameDir;
    double factorMmToPix;
    double distBtFrms;
};

#endif // QFRAMECALINFO_H
