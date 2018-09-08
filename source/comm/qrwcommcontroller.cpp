#include <QMutexLocker>
#include <QSerialPortInfo>
#include <QList>

#include "qrwcommcontroller.h"
#include "FileLogger.h"


QRWCommWorker::QRWCommWorker(QObject *parent) : QObject(parent)
{
    commInst = NULL;

    baud = 9600;
    waitTime = 150;
}

int QRWCommWorker::getFrameIndex(unsigned int& frameIndex)
{
    unsigned short frameIndexs[2] = {};
    if (rtu.readRegisters(ADDR_KZB_FRAME_INDEX, 2, frameIndexs) < 0)
        return -1;

    frameIndex = ((frameIndexs[1] << 16) | (frameIndexs[0] & 0xFFFF)) & 0xFFFFFFFF;

    char msg[1024] = {};
    sprintf(msg, "getFrameIndex, frameIndex[0x%04x 0x%04x]",
            frameIndexs[1], frameIndexs[0]);

    logFile(FileLogger::info, msg);

    return 0;
}

int QRWCommWorker::setInspectResult(int leftResult,
                                    int rightResult,
                                    unsigned int frameIndex)
{
    unsigned short data[4] = {};
    data[0] = productClassToData(leftResult);
    data[1] = productClassToData(rightResult);
    data[2] = frameIndex & 0xFFFF;
    data[3] = (frameIndex >> 16) & 0xFFFF;

    char msg[1024] = {};
    sprintf(msg, "setInspectResult, data[0x%04x 0x%04x 0x%04x 0x%04x]",
            data[0], data[1], data[2], data[3]);

    logFile(FileLogger::info, msg);

    return rtu.writeRegisters(ADDR_LEFTCOL_RESULT, 4, data);
}

int QRWCommWorker::readRegisters(int reg_addr, int reg_num, unsigned short *dest)
{
    QMutexLocker locker(&mutex);
    return rtu.readRegisters(reg_addr, reg_num, dest);
}

int QRWCommWorker::writeRegister(int reg_addr, unsigned short value)
{
    QMutexLocker locker(&mutex);
    return rtu.writeRegister(reg_addr, value);
}

int QRWCommWorker::writeRegisters(int reg_addr, int reg_num, const unsigned short *data)
{
    QMutexLocker locker(&mutex);
    return rtu.writeRegisters(reg_addr, reg_num, data);
}

void QRWCommWorker::deinit()
{
    QMutexLocker locker(&mutex);
    if (commInst) {
        delete commInst;
        commInst = NULL;
    }
}

void QRWCommWorker::onOpenCom(int &comIndex, int *ret)
{
    QMutexLocker locker(&mutex);

    if (commInst) {
        delete commInst;
        commInst = NULL;
    }

    commInst = new SerialComm;
    rtu.setComm(commInst);

    QList<QSerialPortInfo> qPortList = QSerialPortInfo::availablePorts();

    int comCount = qPortList.count();
    if ((comIndex < -1) || (comIndex > comCount - 1)) {
        *ret = -1;
        logFile(FileLogger::warn, "Failed to open the rtu com."
                "The input com index is out of range.");
        return;
    }

    if (-1 == comIndex) {
        for (int iPort = 0; iPort < comCount; iPort++) {
            if (commInst->init(qPortList.at(iPort).portName(),
                               baud, 8, 1, 0, waitTime) == 0) {
                unsigned int frameIndex = 0;
                if (getFrameIndex(frameIndex) == 0) {
                    comIndex = iPort;
                    *ret = 0;
                    return;
                }
            }
        }
    }
    else {
        if (commInst->init(qPortList.at(comIndex).portName(),
                           baud, 8, 1, 0, waitTime) == 0) {
            unsigned int frameIndex = 0;
            if (getFrameIndex(frameIndex) == 0) {
                *ret = 0;
                return;
            }
        }
    }

    //not open sucess
    char message[128] = {};
    sprintf(message, "Failed to open the rtu com."
                     " The input com index is %d.",
            comIndex);
    logFile(FileLogger::warn, message);

    delete commInst;
    commInst = NULL;
    rtu.setComm(commInst);

    *ret = -1;
}

void QRWCommWorker::onOpenComWithBaud(QString &portName, int portBaud, int *ret)
{
    QMutexLocker locker(&mutex);

    if (commInst) {
        delete commInst;
        commInst = NULL;
    }

    commInst = new SerialComm;
    rtu.setComm(commInst);

    if (commInst->init(portName, portBaud, 8, 1, 0, waitTime) == 0) {
        unsigned int frameIndex = 0;
        if (getFrameIndex(frameIndex) == 0) {
            *ret = 0;
            return;
        }
    }

    //not open sucess
    char message[128] = {};
    sprintf(message, "Failed to open the rtu com."
                     " The input port name is %s."
                     " The input baud is %d.",
            portName.toLatin1().data(), portBaud);
    logFile(FileLogger::warn, message);

    delete commInst;
    commInst = NULL;
    rtu.setComm(commInst);

    *ret = -1;
}

void QRWCommWorker::onSigGetFrameIndex(unsigned int &frameIndex, int* ret)
{
    QMutexLocker locker(&mutex);
    *ret = getFrameIndex(frameIndex);
}

void QRWCommWorker::onSigSetInspectResult(int leftResult,
                                          int rightResult,
                                          unsigned int frameIndex,
                                          int *ret)
{
    QMutexLocker locker(&mutex);
    *ret = setInspectResult(leftResult, rightResult, frameIndex);
}

void QRWCommWorker::setSlaveAddr(int slaveAddr)
{
    QMutexLocker locker(&mutex);
    rtu.setSlaveAddr(slaveAddr);
}

unsigned short QRWCommWorker::productClassToData(int productClass)
{
    unsigned short result = 0;
    if (productClass >= PRODUCT_CLASS_0 &&
            productClass <= PRODUCT_CLASS_7) {
        result |= 1 << 15;
        result |= 1 << productClass;
    }

    return result;
}

///
/// \brief QRWCommController::QRWCommController
/// \param parent
///
QRWCommController::QRWCommController(QObject *parent) : QObject(parent)
{
    worker.moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished,
            &worker, &QRWCommWorker::deinit);

    connect(this, &QRWCommController::openCom,
            &worker, &QRWCommWorker::onOpenCom,
            Qt::BlockingQueuedConnection);

    connect(this, &QRWCommController::openComWithBaud,
            &worker, &QRWCommWorker::onOpenComWithBaud,
            Qt::BlockingQueuedConnection);

    connect(this, &QRWCommController::sigGetFrameIndex,
            &worker, &QRWCommWorker::onSigGetFrameIndex,
            Qt::BlockingQueuedConnection);

    connect(this, &QRWCommController::sigSetInspectResult,
            &worker, &QRWCommWorker::onSigSetInspectResult,
            Qt::BlockingQueuedConnection);

    workerThread.start();
}

QRWCommController::~QRWCommController()
{
    workerThread.quit();
    workerThread.wait();
}

int QRWCommController::setComm(QString &portName, int baud)
{
    int ret = 0;
    emit openComWithBaud(portName, baud, &ret);

    return ret;
}

int QRWCommController::setComm(int &comIndex)
{
    int ret = 0;
    emit openCom(comIndex, &ret);

    return ret;
}

int QRWCommController::getFrameIndex(unsigned int& frameIndex)
{
    int ret = -1;
    emit sigGetFrameIndex(frameIndex, &ret);

    return ret;
}

int QRWCommController::setInspectResult(int leftResult, int rightResult,
                                        unsigned int frameIndex)
{
    int ret = -1;
    emit sigSetInspectResult(leftResult, rightResult, frameIndex, &ret);

    return ret;
}

int QRWCommController::readRegisters(int reg_addr, int reg_num, unsigned short *dest)
{
    return worker.readRegisters(reg_addr, reg_num, dest);
}

int QRWCommController::writeRegister(int reg_addr, unsigned short value)
{
    return worker.writeRegister(reg_addr, value);
}

int QRWCommController::writeRegisters(int reg_addr, int reg_num,
                                      const unsigned short *data)
{
    return worker.writeRegisters(reg_addr, reg_num, data);
}

void QRWCommController::setSlaveAddr(int slaveAddr)
{
    worker.setSlaveAddr(slaveAddr);
}
