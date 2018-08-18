#ifndef CMODBUSRTU_H
#define CMODBUSRTU_H

#include "serialcomm.h"
#include <QMutex>

#define BYTESTOSHORT(hbyte,lbyte)   ((short(hbyte)<<8)|(short(lbyte)&0xFF))
#define HIGHBYTE(w)                 ((w>>8)&0xFF)
#define LOWBYTE(w)                  (w&0xFF)

class CModBusRtu
{
public:
    CModBusRtu();

    void setComm(SerialComm* comm);
    void setSlaveAddr(int slaveAddr);

    /**
     *   @brief  读多个寄存器  0x03
     *   最多同时读29个寄存器
     *      reg_addr[in] -- 寄存器起始地址
     *      reg_num[in] -- 寄存器个数
     *      dest[out] -- 寄存器内容
     *    return: 0 - 成功， -1 - 失败
     */
    int readRegisters(int reg_addr, int reg_num, unsigned short *dest);

    /**
     *   @brief  写单个寄存器  0x06
     *    return: 0 - 成功， -1 - 失败
     */
    int writeRegister(int reg_addr, int value);

    /**
     *   @brief  写多个寄存器  0X10
     *    return: 0 - 成功， -1 - 失败
     */
    int writeRegisters(int reg_addr, int reg_num, const unsigned short *data);

private:
    //一次命令及等待响应事务
    int oneTransaction(char* inData, char* outData, qint64 inDataLen,
                       qint64 outDataLen);

private:
    SerialComm* serialComm;
    QMutex mutex;

    int slaveAddr;

};

#endif // CMODBUSRTU_H
