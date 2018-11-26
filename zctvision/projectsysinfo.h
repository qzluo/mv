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

    void load(void);    //load default cfg file
    int save(void);     //save to default cfg file

    int loadFromFile(QString& fileName);
    int saveToFile(QString& fileName);

    int getCameraType() const;
    void setCameraType(int value);

    QString getPortName() const;
    void setPortName(const QString &value);

    int getPortBaud() const;
    void setPortBaud(int value);

    bool getOutputIsOpened() const;
    void setOutputIsOpened(bool value);

    bool getSaveImageIsOpened() const;
    void setSaveImageIsOpened(bool value);

    bool getLogOutputIsOpened() const;
    void setLogOutputIsOpened(bool value);

    QString getTmpFilePath() const;
    void setTmpFilePath(const QString &value);

    QString getAdministratorPw() const;
    void setAdministratorPw(const QString &value);

private:
    void loadElement(QDomElement& element);

private:
    int cameraType;
    QString portName;
    int portBaud;
    bool outputIsOpened;
    bool saveImageIsOpened;
    bool logOutputIsOpened;
    QString tmpFilePath;
    QDateTime closingDateTime;

    QString administratorPw;
};

#endif // PROJECTSYSINFO_H
