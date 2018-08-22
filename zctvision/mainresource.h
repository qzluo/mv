#ifndef MAINRESOURCE_H
#define MAINRESOURCE_H

#include <QObject>

#include "qopencvcapturectl.h"

#include "graphicswidget.h"
#include "qinspectctl.h"
#include "cameraimagecache.h"
#include "qvirtualcameractl.h"

#include "algorithmmanager.h"
#include "qrwcommcontroller.h"
#include "projectsysinfo.h"

//系统状态
enum E_SYS_STATE {
    SYS_STATE_IDLE,     //此状态下释放资源，可以进行相机选择等
    SYS_STATE_STARTED,  //此状态下资源初始化成功，可以进行检测操作
};

enum E_INSPECT_STATE {
    INSPECT_STATE_WAIT,
    INSPECT_STATE_INSPECTED,
};

#define mic MainResource::getInstance()

class MainResource : public QObject
{
    Q_OBJECT

public:
    static MainResource* getInstance();

private:
    explicit MainResource(QObject *parent = 0);

public:    
    ~MainResource();

    int init(void);

    int startSys(void);
    int stopSys(void);

    int startInspect(void);
    int stopInspect(void);

    void initAlg(void);

    ProjectSysInfo* getPSysInfo();
    QFrameCalInfo* getPFrameCalInfo(void);
    QZaoInspectAlgParas* getPInspectAlgParas(void);

    E_SYS_STATE getSysState() const;
    void setSysState(const E_SYS_STATE &value);

    E_INSPECT_STATE getInspectState() const;
    void setInspectState(const E_INSPECT_STATE &value);

    QRWCommController *getRwCommInst() const;
    void setRwCommInst(QRWCommController *value);

private:
    //初始化资源
    int initRc(void);
    int releaseRc(void);

    int updateFrameId(void);

signals:
    void hasImage(const QImage& image);
    void inspectDone(DetectResult detectResult);

public slots:
    void onInspectDone();

private:
    int initCamera(void);
    int initRwComm(void);

private:
    static MainResource* instance;

    E_SYS_STATE sysState;
    E_INSPECT_STATE inspectState;

    CameraCtl* pCamCtl;
    QInspectCtl* inspectObj;
    AlgorithmManager algorithmManager;

    QRWCommController* rwCommInst;

    ProjectSysInfo sysInfo;
};

#endif // MAINRESOURCE_H
