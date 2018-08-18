#include "cmodbusrtutester.h"

#include <QDebug>

CModbusRtuTester::CModbusRtuTester(QObject *parent) : QObject(parent)
{

}

int CModbusRtuTester::init()
{
    if (serialcomm.init("com1") < 0) {
        qDebug() << "CModbusRtuTester::init() -- failed";
        return -1;
    }

    rtu.setComm(&serialcomm);

    qDebug() << "CModbusRtuTester::init() -- success";

    return 0;
}

void CModbusRtuTester::testreadRegisters()
{
    qDebug() << "CModbusRtuTester::testreadRegisters()";

    unsigned short result[10] = {};

    qDebug() << "0x01 0x03 0x00 0x01 0x00 0x01 ";
    int ret = rtu.readRegisters(1, 1, result);
    QString str = "";
    for (int i = 0; i < 1; i++)
        str += QString("0x%1 ").arg(((unsigned short)result[i]) & 0xFFFF, 0, 16);

    qDebug() << str;

    qDebug() << "0x01 0x03 0x00 0x11 0x00 0x02 ";
    ret = rtu.readRegisters(17, 2, result);

    str = "";
    for (int i = 0; i < 2; i++)
        str += QString("0x%1 ").arg(((unsigned short)result[i]) & 0xFFFF, 0, 16);

    qDebug() << str;

    qDebug() << ret;
}

void CModbusRtuTester::testwriteRegister()
{
    qDebug() << "CModbusRtuTester::testwriteRegister()";

    qDebug() << "before write";

    unsigned short result[10] = {};
    int ret = rtu.readRegisters(12, 1, result);
    QString str = "";
    for (int i = 0; i < 1; i++)
        str += QString("0x%1 ").arg(((unsigned short)result[i]) & 0xFFFF, 0, 16);

    qDebug() << str;

    ret = rtu.writeRegister(12, 0x0bcd);
    qDebug() << ret;

    qDebug() << "after write";
    ret = rtu.readRegisters(12, 1, result);
    str = "";
    for (int i = 0; i < 1; i++)
        str += QString("0x%1 ").arg(((unsigned short)result[i]) & 0xFFFF, 0, 16);

    qDebug() << str;

}

void CModbusRtuTester::testwriteRegisters()
{
    qDebug() << "CModbusRtuTester::testwriteRegisters()";

    qDebug() << "before write";

    unsigned short result[10] = {};
    int ret = rtu.readRegisters(17, 2, result);

    QString str = "";
    for (int i = 0; i < 2; i++)
        str += QString("0x%1 ").arg(((unsigned short)result[i]) & 0xFFFF, 0, 16);

    qDebug() << str;

    unsigned short write_data[2] = {0xabcd, 0x5678};
    ret = rtu.writeRegisters(17, 2, write_data);
    qDebug() << ret;

    qDebug() << "after write";
    ret = rtu.readRegisters(17, 2, result);

    str = "";
    for (int i = 0; i < 2; i++)
        str += QString("0x%1 ").arg(((unsigned short)result[i]) & 0xFFFF, 0, 16);

    qDebug() << str;
}
