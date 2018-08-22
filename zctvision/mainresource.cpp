#include "mainresource.h"
#include "FileLogger.h"

MainResource* MainResource::instance = new MainResource();

MainResource *MainResource::getInstance()
{
    return instance;
}

MainResource::MainResource(QObject *parent) : QObject(parent)
{
    sysState = SYS_STATE_IDLE;
    inspectState = INSPECT_STATE_WAIT;

    pCamCtl = NULL;
    rwCommInst = NULL;
}

MainResource::~MainResource()
{
    releaseRc();
}

int MainResource::init()
{
    //read cfg
    sysInfo.load();
    if (sysInfo.load() < 0) {
        logFile(FileLogger::warn, "Load sysinfo file failed!");
        return -1;
    }

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

    return 0;
}

int MainResource::startSys()
{
    if (getSysState() != SYS_STATE_IDLE)
        return -1;

    if (initRc() < 0) {
        logFile(FileLogger::info, "Init rc failed!");
        releaseRc();
        return -1;
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

    cic->startNewCache(1);

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

void MainResource::initAlg()
{
    algorithmManager.reset();
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

    //reset algorithm manager
    if (algorithmManager.reset() < 0) {
        logFile(FileLogger::warn, "Reset algorithm failed!");
        return -3;
    }

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
    DetectResult detectResult = {};
    QVariant var;
    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Inspect_Result),
                        var);
    qDebug() << "inspect return: " << var.toInt();
    detectResult.result = var.toInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Cur_FrameId),
                        var);
    qDebug() << "current FrameId: " << var.toUInt();
    detectResult.frameId = var.toUInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Left_Col_Result),
                        var);
    qDebug() << "E_Left_Col_Result: " << var.toInt();
    detectResult.left_col_result = var.toInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                            QZaoInspectAlgApp::E_Right_Col_Result),
                        var);
    qDebug() << "E_Right_Col_Result: " << var.toInt();
    detectResult.right_col_result = var.toInt();

    algorithmManager.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
                                          QZaoInspectAlgApp::E_Cur_Frame_Product_Info),
                                      var);
    detectResult.curFrameZaoInfo = var;

    char msg[1024] = {};
    if (detectResult.result == 0) {
        //发送结果给串口
        if (rwCommInst->setInspectResult(detectResult.left_col_result,
                                         detectResult.right_col_result,
                                         detectResult.frameId & 0xFFFF) < 0)
            logFile(FileLogger::warn, "Set inspect result to controller failed!");

        sprintf(msg, "Inspect finish. Frame id = %d, Return = %d, "
                "left_col_result = %d, right_col_result = %d.",
                detectResult.frameId, detectResult.result,
                detectResult.left_col_result,
                detectResult.right_col_result);
    }
    else
        sprintf(msg, "Inspect finish. Frame id = %d, Return = %d.",
                detectResult.frameId, detectResult.result);

    logFile(FileLogger::info, msg);

    emit inspectDone(detectResult);
}

int MainResource::initCamera()
{
    int cameraType = sysInfo.getCameraType();
    if (cameraType == ProjectSysInfo::CAMERATYPE_AVT)
        pCamCtl = new QVirtualCameraCtl(this);
    else
        pCamCtl = new QOpenCVCaptureCtl(this);

    if (!pCamCtl->Initiallize(0)) {
        delete pCamCtl;
        pCamCtl = NULL;

        return -1;
    }

    pCamCtl->StartView();

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
    int comIndex = sysInfo.getSerialPortNo();
    if (rwCommInst->setComm(comIndex) < 0)
        return -1;

    if (comIndex != sysInfo.getSerialPortNo()) {
        sysInfo.setSerialPortNo(comIndex);
        sysInfo.save();
    }

    return 0;
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
