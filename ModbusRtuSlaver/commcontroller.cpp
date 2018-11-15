#include "commcontroller.h"
#include "public.h"
#include <QTimerEvent>
#include <QMutexLocker>

#include <QDebug>

static YCDATA s_dbPlcData[] = {
    {0, "M0000", 0},
    {1, "M0001", 0},
    {2, "M0002", 0},
    {3, "M0003", 0},
    {4, "M0004", 0},
    {5, "M0005", 0},
    {6, "M0006", 0},
    {7, "M0007", 0},

    {8, "M0100", 0},
    {9, "M0101", 0},

    {10, "M0102", 0},
    {11, "M0103", 0},
    {12, "M0104", 0},
    {13, "M0105", 0},
    {14, "M0106", 0},
    {15, "M0107", 0},
    {16, "M0108", 0},
    {17, "M0109", 0x1213},

    {18, "M0110", 0x1514},
    {19, "M0111", 0},
    {20, "M0112", 0},
    {21, "M0113", 0},
    {22, "M0114", 0},
    {23, "M0115", 0},
    {24, "M0116", 0},
    {25, "M0117", 0},
    {26, "M0118", 0},
    {27, "M0119", 0},

    {28, "X0000", 0},
    {29, "X0001", 0},
    {30, "X0002", 0},
    {31, "X0003", 0},
    {32, "X0004", 0},
    {33, "X0005", 0},
    {34, "X0006", 0},
    {35, "X0007", 0},

    {36, "M0118", 0},
    {37, "M0119", 0},

    {38, "X0000", 0},
    {39, "X0001", 0},
    {40, "X0002", 0},
    {41, "X0003", 0},

};

static PYCDATA getYcData(int addr)
{
    for (int i = 0; i < sizeof(s_dbPlcData) / sizeof(s_dbPlcData[0]); ++i) {
        if (s_dbPlcData[i].no == addr)
            return &s_dbPlcData[i];
    }

    return NULL;
}

static int setYcDataVal(int addr, int value)
{
    PYCDATA pData = NULL;
    pData = getYcData(addr);

    if (!pData)
        return -1;

    pData->value = value;
    return 0;
}

static int getYcDataVal(int addr, int* value)
{
    PYCDATA pData = NULL;
    pData = getYcData(addr);

    if (!pData)
        return -1;

    *value = pData->value;
    return 0;
}

static unsigned short const wCRC16Table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

void CRC16_Modbus(const unsigned char* pDataIn, int iLenIn, unsigned short* pCRCOut)
{
     unsigned char wTableNo = 0;
     unsigned short crc16 = 0xFFFF;
     for ( auto i = 0; i < iLenIn; ++i ) {
         wTableNo = pDataIn[i] ^ crc16;
         crc16 >>= 8;
         crc16 ^= wCRC16Table[ wTableNo ];
     }

     *pCRCOut = crc16;
}

///
/// \brief CommWorker::CommWorker
/// \param parent
///
CommWorker::CommWorker(QObject *parent) : QObject(parent)
{
    serialComm = NULL;
}

void CommWorker::setRegData(int reg_addr, unsigned short value)
{
    QMutexLocker lock(&mutex);

    setYcDataVal(reg_addr, value);

    int new_value = 0;
    getYcDataVal(reg_addr, &new_value);

    qDebug() << QString("addr: %1, new_value: 0x%2").
                arg(reg_addr).
                arg(new_value & 0xFFFF, 0, 16);
}

int CommWorker::readRegAdd(int reg_addr, unsigned short *value)
{
    QMutexLocker lock(&mutex);
    int ycValue = 0;
    int ret = getYcDataVal(reg_addr, &ycValue);
    if (ret == 0)
        *value = ycValue & 0xFFFF;

    return ret;

}

void CommWorker::init()
{
    QMutexLocker lock(&mutex);
    serialComm = new SerialComm();
    if (serialComm->init("com2", 9600, 8, 1, 0, 10)) {
        qDebug() << "failed to open comm com2." << endl;
        return;
    }

    readTimerId = startTimer(COMMWORKER_DELAY_TIME);
}

void CommWorker::deinit()
{
    QMutexLocker lock(&mutex);
    if (!serialComm) {
        serialComm->close();
        serialComm = NULL;
    }
}

void CommWorker::onCommNameChanged(QString comName)
{
    QMutexLocker lock(&mutex);
    if (!serialComm) {
        serialComm->close();
        serialComm = NULL;
    }

    if (serialComm->init(comName, 9600, 8, 1, 0, 10)) {
        qDebug() << "failed to open comm " << comName << "." << endl;
        return;
    }
}

void CommWorker::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == readTimerId) {
        /* do the following works:
         * 1. read data from io
         * 2. show in the interface
         * 3. write back to the peer
         * 4. sleep
         * */

        char readBuf[1024] = {};
        int readCount = 0;

        QMutexLocker lock(&mutex);
        if (!serialComm->isOpen())
            return ;

        //read data
        readCount = serialComm->read(readBuf, 1024);
        if (readCount <= 0)
            return;

        //show in the interface
        handleInput(readBuf, readCount);
    }
}

void CommWorker::handleInput(char *input, int dataLen)
{
    QString str;
    for (int i = 0; i < dataLen; i++)
        str += QString("0x%1 ").arg(((int)input[i]) & 0xFF, 0, 16);

    emit hasCommInput(str);

    //parse input
    char outBuf[512] = {};
    int writeDataLen = 0;
    if (dataLen > 2) {
        if (input[1] == 0x03) {
            if (handleReadRegister(input, dataLen, outBuf, &writeDataLen) < 0)
                return;
        }
        else if (input[1] == 0x06) {
            if (handleWriteRegister(input, dataLen, outBuf, &writeDataLen) < 0)
                return;
        }
        else if (input[1] == 0x10) {
            if (handleWriteRegisters(input, dataLen, outBuf, &writeDataLen) < 0)
                return;
        }

    }

    serialComm->write(outBuf, writeDataLen);
    str = "";
    for (int i = 0; i < writeDataLen; i++)
        str += QString("0x%1 ").arg(((int)outBuf[i]) & 0xFF, 0, 16);

    emit hasCommInput(str);
}

///
/// \brief CModBusRtu::readRegisters
/// 指令： 0x01(从机地址） 0x03(功能码） 0xXX(寄存器地址高八位） 0xXX(寄存器地址低八位）
///       0xXX（寄存器数量高八位） 0xXX（寄存器数量低八位） CRCL CRCH
/// 回应: 0x01 0x03 0xXX(返回字节数) Data1H Data1L ... CRCL CRCH
///
int CommWorker::handleReadRegister(char *input, int dataLen, char *output, int *outLen)
{
    if (dataLen != 8)
        return -1;

    if (input[1] != 3)
        return -1;

    int reg_addr = BYTESTOSHORT(input[2], input[3]);
    int reg_number = BYTESTOSHORT(input[4], input[5]);

    int values[16] = {};

    for (int i = 0; i < reg_number; ++i) {
        int value = 0;
        if (getYcDataVal(reg_addr + i, &value) == 0) {
            values[i] = value;
        }
    }

    //计算输出
    *outLen = 5 + 2 * reg_number;
    output[0] = input[0];
    output[1] = input[1];
    output[2] = 2 * reg_number;
    for (int i = 0; i < reg_number; ++i) {
        output[3 + 2 * i] = HIGHBYTE(values[i]);
        output[4 + 2 * i] = LOWBYTE(values[i]);
    }

    unsigned short crcData = 0;
    CRC16_Modbus((unsigned char*)output, 3 + 2 * reg_number, &crcData);
    output[3 + 2 * reg_number] = LOWBYTE(crcData);
    output[4 + 2 * reg_number] = HIGHBYTE(crcData);

    return 0;
}

/// 指令： 0x01(从机地址） 0x06(功能码） 0xXX(寄存器地址高八位） 0xXX(寄存器地址低八位）
///       DataH DataL CRCL CRCH
/// 回应: 同指令
int CommWorker::handleWriteRegister(char *input, int dataLen, char *output, int *outLen)
{
    if (dataLen != 8)
        return -1;

    if (input[1] != 6)
        return -1;

    int reg_addr = BYTESTOSHORT(input[2], input[3]);
    int value = BYTESTOSHORT(input[4],input[5]);
    if (setYcDataVal(reg_addr, value) == 0) {
        //update to ui
        if (reg_addr == 1) {
            emit sigDbRegChanged(reg_addr, value & 0xFFFF);
        }
        else if (reg_addr == 2){
            emit sigDbRegChanged(reg_addr, value & 0xFFFF);
        }
        else if (reg_addr == 3 || reg_addr == 4) {
            //软件图片序号
            int sft_frame_ids[2] = {};
            getYcDataVal(3, &sft_frame_ids[0]);
            getYcDataVal(4, &sft_frame_ids[1]);

            unsigned int sft_frame_id = 0;
            sft_frame_id = sft_frame_ids[0] & 0xFFFF +
                    (sft_frame_ids[1] << 16) & 0xFFFFFFFF;

            emit sigDbFrameIdChanged(3, sft_frame_id);
        }
        else if (reg_addr == 5 || reg_addr == 6) {
            //控制板图片序号
            int frame_ids[2] = {};
            getYcDataVal(5, &frame_ids[0]);
            getYcDataVal(6, &frame_ids[1]);

            unsigned int frame_id = 0;
            frame_id = frame_ids[0] & 0xFFFF +
                    (frame_ids[1] << 16) & 0xFFFFFFFF;

            emit sigDbFrameIdChanged(5, frame_id);
        }
    }

    //计算输出
    *outLen = dataLen;
    for (int i = 0; i < dataLen; ++i) {
        output[i] = input[i];
    }

    return 0;
}

/// 指令： 0x01(从机地址） 0x10(功能码） 0xXX(寄存器地址高八位） 0xXX(寄存器地址低八位）
///       0xXX（寄存器数量高八位） 0xXX（寄存器数量低八位）0xXX(字节数)
///       Data1H Data1L ... CRCL CRCH
/// 回应:  0x01 0x10 0xXX(寄存器地址高八位） 0xXX(寄存器地址低八位）
///       0xXX（寄存器数量高八位） 0xXX（寄存器数量低八位）CRCL CRCH
int CommWorker::handleWriteRegisters(char *input, int dataLen, char *output, int *outLen)
{
    if (dataLen < 9)
        return -1;

    if (input[1] != 0x10)
        return -1;

    int reg_addr = BYTESTOSHORT(input[2], input[3]);
    int reg_number = BYTESTOSHORT(input[4], input[5]);

    for (int i = 0; i < reg_number; ++i) {
        unsigned short value = BYTESTOSHORT(input[7 + 2*i], input[8 + 2*i]);
        setYcDataVal(reg_addr+i, value);

        if (reg_addr <= 1 && reg_addr + reg_number - 1 >= 1) {
            int value = 0;
            getYcDataVal(1, &value);
            emit sigDbRegChanged(1, value & 0xFFFF);
        }

        if (reg_addr <= 2 && reg_addr + reg_number - 1 >= 2) {
            int value = 0;
            getYcDataVal(2, &value);
            emit sigDbRegChanged(2, value & 0xFFFF);
        }

        //update to ui
        if ((reg_addr <= 3 && reg_addr + reg_number - 1 >= 3) ||
                (reg_addr <= 4 && reg_addr + reg_number - 1 >= 4)) {
            //控制板图片序号
            int frame_ids[2] = {};
            getYcDataVal(3, &frame_ids[0]);
            getYcDataVal(4, &frame_ids[1]);

            unsigned int frame_id = 0;
            frame_id = frame_ids[0] & 0xFFFF +
                    (frame_ids[1] << 16) & 0xFFFFFFFF;

            emit sigDbFrameIdChanged(3, frame_id);
        }

        if ((reg_addr <= 5 && reg_addr + reg_number - 1 >= 5) ||
                (reg_addr <= 5 && reg_addr + reg_number - 1 >= 5)) {
            //控制板图片序号
            int frame_ids[2] = {};
            getYcDataVal(5, &frame_ids[0]);
            getYcDataVal(6, &frame_ids[1]);

            unsigned int frame_id = 0;
            frame_id = frame_ids[0] & 0xFFFF +
                    (frame_ids[1] << 16) & 0xFFFFFFFF;

            emit sigDbFrameIdChanged(5, frame_id);
        }
    }

    //计算输出
    *outLen = 8;
    for (int i = 0; i < 6; ++i) {
        output[i] = input[i];
    }

    unsigned short crcData = 0;
    CRC16_Modbus((unsigned char*)output, 6, &crcData);
    output[6] = LOWBYTE(crcData);
    output[7] = HIGHBYTE(crcData);

    return 0;
}


//////////////////////////////////////////////////////////////
/// CommController

CommController::CommController(QObject *parent) : QObject(parent)
{
    worker.moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(started()), &worker, SLOT(init()));
    connect(&workerThread, SIGNAL(finished()), &worker, SLOT(deinit()));
    connect(&worker, SIGNAL(hasCommInput(QString)), this, SIGNAL(hasData(QString)));
    connect(&worker, &CommWorker::sigDbRegChanged,
            this, &CommController::sigDbRegChanged);
    connect(&worker, &CommWorker::sigDbFrameIdChanged,
            this, &CommController::sigDbFrameIdChanged);

    connect(this, SIGNAL(commNameChanged(QString)),
            &worker, SLOT(onCommNameChanged(QString)));

    workerThread.start();
}

CommController::~CommController()
{
    workerThread.quit();
    workerThread.wait();
}

int CommController::readRegAdd(int reg_addr, unsigned short *value)
{
    return worker.readRegAdd(reg_addr, value);
}

void CommController::onSigRegDataSet(int reg_addr, unsigned short value)
{
    worker.setRegData(reg_addr, value);
}

