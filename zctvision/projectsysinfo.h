#ifndef PROJECTSYSINFO_H
#define PROJECTSYSINFO_H

#include <QDateTime>
#include <QtXml>

class ProjectSysInfo
{
public:
    ProjectSysInfo();

    enum ECameraType {
        CAMERATYPE_NONE = 0,
        CAMERATYPE_AVT,
        CAMERATYPE_DAHENG,
    };

    int load(void);
    int save(void);

    int getCameraType() const;
    void setCameraType(int value);

    QString getPortName() const;
    void setPortName(const QString &value);

    int getPortBaud() const;
    void setPortBaud(int value);

private:
    void loadElement(QDomElement& element);

private:
    int cameraType;
    QString portName;
    int portBaud;
    QDateTime closingDateTime;
};

#endif // PROJECTSYSINFO_H
