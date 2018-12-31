#include "mainresource.h"
#include "FileLogger.h"

#include <QJsonDocument>
#include <QJsonObject>

MainResource* MainResource::instance = new MainResource();

MainResource *MainResource::getInstance()
{
    return instance;
}

MainResource::MainResource(QObject *parent) : QObject(parent)
{
    sysState = SYS_STATE_IDLE;
    inspectState = INSPECT_STATE_WAIT;

    inspectCount = 0;
    pCamCtl = NULL;
    rwCommInst = NULL;

    role = ROLE_USER;
}

MainResource::~MainResource()
{
    releaseRc();
}

int MainResource::init()
{
    //read cfg
    sysInfo.load();

    inspectObj = new QInspectCtl(this);
    inspectObj->initThread();
    inspectObj->setAlgorithmManager(&algorithmManager);

    qRegisterMetaType<DetectResult>("DetectResult");

    connect(inspectObj, &QInspectCtl::inspectDone,
            this, &MainResource::onInspectDone);

    if (algorithmManager.init() < 0) {
        logFile(FileLogger::warn, "Init algorithm failed!");
        return -1;
    }

    //udp data handle
    connect(&udpServer, &QUdpServer::receiveDatagrams,
            this, &MainResource::onReceiveDatagrams);

    return 0;
}

int MainResource::startSys()
{
    if (getSysState() != SYS_STATE_IDLE)
        return -1;

    int ret = initRc();
    if (ret < 0) {
        logFile(FileLogger::info, "Init rc failed!");
        releaseRc();
        return ret;
    }

    logFile(FileLogger::info, "startSys.");

    setSysState(SYS_STATE_STARTED);
    setInspectState(INSPECT_STATE_WAIT);

    return 0;
}

int MainResource::stopSys()
{
    if (getSysState() != SYS_STATE_STARTED)
        return -1;

    if (releaseRc() < 0)
        return -1;

    logFile(FileLogger::info, "stopSys.");

    setSysState(SYS_STATE_IDLE);
    setInspectState(INSPECT_STATE_WAIT);

    return 0;
}

int MainResource::startInspect()
{
    if (getSysState() != SYS_STATE_STARTED)
        return -1;

    if (getInspectState() != INSPECT_STATE_WAIT)
        return -1;

    //reset algorithm image size
    int imgWidth = 0;
    int imgHeight = 0;
    cic->getImageSize(&imgWidth, &imgHeight);
    if (algorithmManager.resetImageSize(imgWidth, imgHeight) < 0) {
        logFile(FileLogger::warn, "ResetImageSize failed!");
        return -1;
    }

    cic->startNewInspect();

    //读当前帧序号并保存
    updateFrameId();

    setInspectState(INSPECT_STATE_INSPECTED);

    return 0;
}

int MainResource::stopInspect()
{
    if (getSysState() != SYS_STATE_STARTED)
        return -1;

    if (getInspectState() != INSPECT_STATE_INSPECTED)
        return -1;

    setInspectState(INSPECT_STATE_WAIT);

    return 0;
}

void MainResource::updateFrameDist()
{
    algorithmManager.updateFrameDist();
}

void MainResource::resetInspectParas()
{
    algorithmManager.resetInspectParas();
}

int MainResource::loadCfgFile(QString &fileName)
{
    return sysInfo.loadFromFile(fileName);
}

int MainResource::saveCfgFile(QString &fileName)
{
    return sysInfo.saveToFile(fileName);
}

int MainResource::reloadAlgParasFile(QString &fileName)
{
    return algorithmManager.reloadAlgParasFile(fileName);
}

int MainResource::saveAlgParas()
{
    return algorithmManager.saveAlgParas();
}

int MainResource::saveAlgParasToFile(QString& fileName)
{
    return algorithmManager.saveAlgParasToFile(fileName);
}

int MainResource::setCameraParas(QString featureName, QString value)
{
    if (!pCamCtl)
        return -1;

    return pCamCtl->SetPara(featureName, value);
}

int MainResource::inspectSingleImage(const QImage &image, QList<ZaoInfo> &cur_left_col_result, QList<ZaoInfo> &cur_right_col_result)
{
    inspectCount++;
    algorithmManager.resetImageSize(image.width(), image.height());

    return algorithmManager.inspectSingleImage(image, cur_left_col_result,
                                               cur_right_col_result);
}

int MainResource::initRc()
{
    //initial camera
    if (initCamera() < 0) {
        logFile(FileLogger::warn, "Initalize camera failed!");
        return -1;
    }

    //init comm
    if (initRwComm() < 0) {
        logFile(FileLogger::warn, "Initalize com failed!");
        return -2;
    }

    //init udp server
    udpServer.setNetworkSegment(sysInfo.getNetworkSegment());
    if (udpServer.initSocket() < 0) {
        logFile(FileLogger::warn, "Initalize udp server failed!");
        return -3;
    }

    udpClient.setNetworkSegment(sysInfo.getNetworkSegment());

    return 0;
}

int MainResource::releaseRc()
{
    if (pCamCtl) {
        pCamCtl->StopView();
        pCamCtl->disconnect();
        delete pCamCtl;
        pCamCtl = NULL;
    }

    if (rwCommInst) {
        delete rwCommInst;
        rwCommInst = NULL;
    }

    udpServer.closeSocket();

    return 0;
}

int MainResource::updateFrameId()
{
    if (!rwCommInst)
        return -1;

    unsigned int frameIndex = 0;
    if (rwCommInst->getFrameIndex(frameIndex) < 0) {
        logFile(FileLogger::warn, "Get frame id from controller failed!");
        return -1;
    }

    cic->updateFrameId(frameIndex);

    return 0;
}

void MainResource::onInspectDone()
{
    if (getSysState() != SYS_STATE_STARTED)
        return ;

    //inspect done
    DetectResult detectResult = {};
    QVariant var;
    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Inspect_Result), var);
    detectResult.result = var.toInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Cur_FrameId), var);
    detectResult.frameId = var.toUInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Left_Grade_Result_ToSend), var);
    detectResult.left_col_grade_result = var.toInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Right_Grade_Result_ToSend), var);
    detectResult.right_col_grade_result = var.toInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                                        QZaoInspectAlgApp::E_Cur_Frame_Result_info), var);
    detectResult.curFrameZaoInfo = var;

    char msg[1024] = {};
    if (detectResult.result < 0) {
        sprintf(msg, "Inspect finish. Frame id = %d, Return = %d.",
                detectResult.frameId, detectResult.result);

        logFile(FileLogger::info, msg);
    }
    else {
        sprintf(msg, "Inspect finish. Frame id = %d, Return = %d, "
                "left_col_grade_result = %d, right_col_grade_result = %d.",
                detectResult.frameId, detectResult.result,
                detectResult.left_col_grade_result,
                detectResult.right_col_grade_result);

        logFile(FileLogger::info, msg);

        if (sysInfo.getOutputIsOpened()) {
            //发送结果给串口
            if (!rwCommInst) {
                logFile(FileLogger::warn, "Comm is not opened!");
                return;
            }

            if (rwCommInst->setInspectResult(detectResult.left_col_grade_result,
                                             detectResult.right_col_grade_result,
                                             detectResult.frameId & 0xFFFF) < 0)
                logFile(FileLogger::warn, "Set inspect result to controller failed!");
        }

        if (sysInfo.getLogOutputIsOpened()) {
            //将结果保存到指定文件
            QString curDate = QDate::currentDate().toString("yyMMdd");
            QString filePath = QString("%1\\outputResult\\").
                    arg(sysInfo.getTmpFilePath());

            QDir dir;
            if (!dir.exists(filePath)) {
                if (!dir.mkpath(filePath)) {
                    qDebug() << "failed to create dir";
                    return;
                }
            }

            QString fileName = filePath + QString("%1.dat").arg(curDate);

            QFile f(fileName);
            if(!f.open(QIODevice::Append | QIODevice::Text)) {
                qDebug() << "open file failed";
            }

            QString strMsg = QString("%1%2").
                    arg(QDateTime::currentDateTime().toString("[HH:mm::ss]")).arg(msg);

            QTextStream txtOutput(&f);
            txtOutput << strMsg << endl;
            f.close();
        }
    }

    inspectCount++;

    logFile(FileLogger::info, "emit signal inspectDone");
    emit inspectDone(detectResult);
}

void MainResource::onReceiveDatagrams(const QByteArray &datagram)
{
    logFile(FileLogger::info, "Receive algorithm parameter datagram");

    //update algrithm parameters
    getPInspectAlgParas()->updateParasWithIncrement(datagram);
}

int MainResource::initCamera()
{
    int cameraType = sysInfo.getCameraType();

    //for temper debug
    if (cameraType == ProjectSysInfo::CAMERATYPE_NONE)
        return 0;

    if (cameraType == ProjectSysInfo::CAMERATYPE_AVT)
        pCamCtl = new QVmbCameraclt(this);
    else if (cameraType == ProjectSysInfo::CAMERATYPE_DAHENG)
        pCamCtl = new QDahengCameraCtl(this);
    else
        return 0;

    if (!pCamCtl->Initiallize(0)) {
        delete pCamCtl;
        pCamCtl = NULL;

        return -1;
    }

    if (!pCamCtl->StartView()) {
        delete pCamCtl;
        pCamCtl = NULL;

        return -1;
    }

    //connect
    connect(pCamCtl, &CameraCtl::hasImage,
            this, &MainResource::hasImage);

    connect(pCamCtl, &CameraCtl::hasImage,
            cic,
            &CameraImageCache::onHasImage);

    cic->setImageSize(pCamCtl->GetCamWidth(),
                      pCamCtl->GetCamHeight());

    return 0;
}

int MainResource::initRwComm()
{
    if (rwCommInst) {
        delete rwCommInst;
        rwCommInst = NULL;
    }

    rwCommInst = new QRWCommController(this);
    QString portName = sysInfo.getPortName();
    int baud = sysInfo.getPortBaud();
    if (rwCommInst->setComm(portName, baud) < 0)
        return -1;

    return 0;
}

int MainResource::getRole() const
{
    return role;
}

void MainResource::setRole(int value)
{
    if (value < ROLE_USER || value > ROLE_ADMINISTRATOR)
        return;

    role = value;
}

int MainResource::broadcastUdpDatagram(const char *data, qint64 size)
{
    return udpClient.broadcastUdpDatagram(data, size);
}

int MainResource::getInspectCount() const
{
    return inspectCount;
}

void MainResource::setInspectCount(int value)
{
    inspectCount = value;
}

unsigned long MainResource::getInspectTime() const
{
    return algorithmManager.getInspectTime();
}

int MainResource::getCameraType()
{
    return sysInfo.getCameraType();
}

QString MainResource::getAdministratorPw()
{
    return sysInfo.getAdministratorPw();
}

CameraCtl *MainResource::getPCamCtl() const
{
    return pCamCtl;
}

void MainResource::setPCamCtl(CameraCtl *value)
{
    pCamCtl = value;
}

QRWCommController *MainResource::getRwCommInst() const
{
    return rwCommInst;
}

void MainResource::setRwCommInst(QRWCommController *value)
{
    rwCommInst = value;
}

E_INSPECT_STATE MainResource::getInspectState() const
{
    return inspectState;
}

void MainResource::setInspectState(const E_INSPECT_STATE &value)
{
    inspectState = value;
}

E_SYS_STATE MainResource::getSysState() const
{
    return sysState;
}

void MainResource::setSysState(const E_SYS_STATE &value)
{
    sysState = value;
}

ProjectSysInfo* MainResource::getPSysInfo()
{
    return &sysInfo;
}

QFrameCalInfo *MainResource::getPFrameCalInfo()
{
    return algorithmManager.getPFrameCalInfo();
}

QZaoInspectAlgParas *MainResource::getPInspectAlgParas()
{
    return algorithmManager.getPInspectAlgParas();
}
