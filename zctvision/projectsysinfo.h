#ifndef PROJECTSYSINFO_H
#define PROJECTSYSINFO_H

#include <QDateTime>
#include <QtXml>

class ProjectSysInfo
{
public:
    ProjectSysInfo();

    enum ECameraType {
        CAMERATYPE_AVT = 0,
        CAMERATYPE_DAHENG,
    };

    int load(void);
    int save(void);

    int getCameraType() const;
    void setCameraType(int value);

    int getSerialPortNo() const;
    void setSerialPortNo(int value);

private:
    void loadElement(QDomElement& element);

private:
    int cameraType;
    int serialPortNo;
    QDateTime closingDateTime;
};

#endif // PROJECTSYSINFO_H
