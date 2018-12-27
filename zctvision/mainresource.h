#ifndef MAINRESOURCE_H
#define MAINRESOURCE_H

#include <QObject>

#include "qopencvcapturectl.h"

#include "graphicswidget.h"
#include "qinspectctl.h"
#include "cameraimagecache.h"
#include "qvirtualcameractl.h"
#include "qvmbcameraclt.h"
#include "qdahengcameractl.h"

#include "qzaoinspectalgapp.h"
#include "qrwcommcontroller.h"
#include "projectsysinfo.h"
#include "qudpserver.h"

//系统状态
enum E_SYS_STATE {
    SYS_STATE_IDLE,     //此状态下释放资源，可以进行相机选择等
    SYS_STATE_STARTED,  //此状态下资源初始化成功，可以进行检测操作
};

enum E_INSPECT_STATE {
    INSPECT_STATE_WAIT,
    INSPECT_STATE_INSPECTED,
};

enum E_ROLE {
    ROLE_USER,
    ROLE_ADMINISTRATOR,
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

    void updateFrameDist(void);
    void resetInspectParas();

    //加载、保存配置文件
    int loadCfgFile(QString& fileName);
    int saveCfgFile(QString& fileName);

    //加载、保存检测参数文件
    int reloadAlgParasFile(QString& fileName);
    int saveAlgParas(void);
    int saveAlgParasToFile(QString& fileName);

    int setCameraParas(QString featureName, QString value);

    //检测单张图片
    int inspectSingleImage(const QImage& image,
                           QList<ZaoInfo>& cur_left_col_result,
                           QList<ZaoInfo>& cur_right_col_result);

    ProjectSysInfo* getPSysInfo();
    QFrameCalInfo* getPFrameCalInfo(void);
    QZaoInspectAlgParas* getPInspectAlgParas(void);

    E_SYS_STATE getSysState() const;
    void setSysState(const E_SYS_STATE &value);

    E_INSPECT_STATE getInspectState() const;
    void setInspectState(const E_INSPECT_STATE &value);

    QRWCommController *getRwCommInst() const;
    void setRwCommInst(QRWCommController *value);

    CameraCtl *getPCamCtl() const;
    void setPCamCtl(CameraCtl *value);

    int getInspectCount() const;
    void setInspectCount(int value);

    unsigned long getInspectTime() const;

    int getCameraType(void);
    QString getAdministratorPw(void);

    int getRole() const;
    void setRole(int value);

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
    void onReceiveDatagrams(const QByteArray& datagram);

private:
    int initCamera(void);
    int initRwComm(void);

private:
    static MainResource* instance;

    E_SYS_STATE sysState;
    E_INSPECT_STATE inspectState;

    int inspectCount;   //检测张数，统计信息用

    CameraCtl* pCamCtl;
    QInspectCtl* inspectObj;
    QZaoInspectAlgApp algorithmManager;

    QRWCommController* rwCommInst;

    ProjectSysInfo sysInfo;

    int role;

    //同步算法参数用
    QUdpServer udpServer;
};

#endif // MAINRESOURCE_H
