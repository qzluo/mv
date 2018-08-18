#ifndef QRWCOMMCONTROLLER_H
#define QRWCOMMCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "serialcomm.h"
#include "cmodbusrtu.h"
#include "qsysdefine.h"


#define ADDR_LEFTCOL_RESULT     0x01
#define ADDR_KZB_FRAME_INDEX    0x05


#define PRODUCT_CLASS_NONE     ZAO_CLASS_NONE  //无枣

#define PRODUCT_CLASS_0        ZAO_CLASS_GOOD1   //特等枣
#define PRODUCT_CLASS_1        ZAO_CLASS_GOOD2   //一等枣
#define PRODUCT_CLASS_2        ZAO_CLASS_GOOD3   //二等枣
#define PRODUCT_CLASS_3        ZAO_CLASS_GOOD4   //三等枣
#define PRODUCT_CLASS_4        ZAO_CLASS_BAD1   //皮皮枣
#define PRODUCT_CLASS_5        ZAO_CLASS_BAD2   //裂口枣
#define PRODUCT_CLASS_6        ZAO_CLASS_BAD3   //鸟啄枣
#define PRODUCT_CLASS_7        ZAO_CLASS_BAD4   //烂枣


class QRWCommWorker : public QObject
{
    Q_OBJECT

public:
    explicit QRWCommWorker(QObject *parent = 0);

    //获得图片帧序号 -1 -- 失败, 0 -- success
    int getFrameIndex(unsigned int& frameIndex);

    ///
    /// \brief setInspectResult: 写检测结果
    /// \param leftResult：左列枣检测结果
    /// \param rightResult：右列枣检测结果
    /// \param frameIndex：图片对应的帧序号
    /// \return   -1 -- 失败, 0 -- success
    ///
    int setInspectResult(int leftResult, int rightResult, unsigned int frameIndex);
    int readRegisters(int reg_addr, int reg_num, unsigned short *dest);
    int writeRegister(int reg_addr, unsigned short value);
    int writeRegisters(int reg_addr, int reg_num, const unsigned short *data);

public slots:
    void deinit();
    void onOpenCom(int &comIndex, int *ret);
    void onSigGetFrameIndex(unsigned int& frameIndex, int* ret);
    void onSigSetInspectResult(int leftResult, int rightResult,
                               unsigned int frameIndex, int* ret);

    void setSlaveAddr(int slaveAddr);

private:
    //将枣等级转化为对应的协议相关数据
    unsigned short productClassToData(int productClass);

private:
    CModBusRtu rtu;
    SerialComm* commInst;

    QMutex mutex;

    int baud;
    int waitTime;
};

class QRWCommController : public QObject
{
    Q_OBJECT
public:
    explicit QRWCommController(QObject *parent = 0);
    ~QRWCommController();

    /**
     * @brief setComm: 设置通信设备
     * @param comIndex[in | out]: 设备索引，当为-1时，将遍历系统找到跟从机通信的串口
     * @return: 0 -- 成功， <0 -- 失败
     */
    int setComm(int& comIndex);

    int getFrameIndex(unsigned int& frameIndex);

    int setInspectResult(int leftResult, int rightResult,
                         unsigned int frameIndex);

    int readRegisters(int reg_addr, int reg_num, unsigned short *dest);
    int writeRegister(int reg_addr, unsigned short value);
    int writeRegisters(int reg_addr, int reg_num, const unsigned short *data);

    void setSlaveAddr(int slaveAddr);

signals:
    void openCom(int& comIndex, int *ret);
    void sigGetFrameIndex(unsigned int& frameIndex, int* ret);
    void sigSetInspectResult(int leftResult, int rightResult,
                             unsigned int frameIndex, int* ret);

public slots:


private:
    QThread workerThread;
    QRWCommWorker worker;
};

#endif // QRWCOMMCONTROLLER_H
