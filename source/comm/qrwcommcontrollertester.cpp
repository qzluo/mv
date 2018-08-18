#include "qrwcommcontrollertester.h"

#include <QDebug>


QRwCommControllerTester::QRwCommControllerTester(QObject *parent) : QObject(parent)
{

}

void QRwCommControllerTester::init()
{
    qDebug() << "QRwCommControllerTester::init()";
    int comIndex = -1;
    int ret = controller.setComm(comIndex);

    qDebug() << "comIndex: " << comIndex << ", ret: " << ret;
}

void QRwCommControllerTester::testGetFrameIndex()
{
    unsigned int frameIndex = 0;
    int ret = controller.getFrameIndex(frameIndex);

    qDebug() << "frame index: " << QString("ret: %1, frameIndex: 0x%2").
                arg(ret).arg((unsigned int)frameIndex, 0, 16);

}

void QRwCommControllerTester::testSetInspectResult()
{
    unsigned short data[4] = {};
    controller.readRegisters(1, 4, data);


    for (int i = 0; i < 4; i++) {
        qDebug() << "i: " << i << ", data: " << QString("0x%1").arg(data[i] & 0xFFFF, 0, 16);
    }

    unsigned int frameIndex = 15;
    int leftResult = 0;
    int rightResult = 1;
    int ret = controller.setInspectResult(leftResult, rightResult, frameIndex);

    qDebug() << "frame index: " << QString("ret: %1, frameIndex: 0x%2").
                arg(ret).arg((unsigned int)frameIndex, 0, 16);

    controller.readRegisters(1, 4, data);

    for (int i = 0; i < 4; i++) {
        qDebug() << "i: " << i << ", data: " << QString("0x%1").arg(data[i] & 0xFFFF, 0, 16);
    }
}

void QRwCommControllerTester::testWriteRegister()
{
    controller.writeRegister(1, 0x1234);
    controller.writeRegister(2, 0x5678);

    controller.writeRegister(3, 10);
    controller.writeRegister(5, 100);
}

void QRwCommControllerTester::testWriteRegisters()
{
    unsigned short data[7] = {
        0x1234,
        0x5678,
        0x11,
        0x0,
        0x65,
        0x0,
        0xabcd,
    };

    controller.writeRegisters(1, 7, data);
}

void QRwCommControllerTester::testProductClassToData()
{
    for (int i = 0; i < 10; i++) {
        unsigned short data = productClassToData(i);
        qDebug() << "i: " << i << ", data: " << QString("0x%1").arg(data & 0xFFFF, 0, 16);

    }
}

unsigned short QRwCommControllerTester::productClassToData(int productClass)
{
    unsigned short result = 0;
    if (productClass >= PRODUCT_CLASS_0 &&
            productClass <= PRODUCT_CLASS_7) {
        result |= 1 << 15;
        result |= 1 << productClass;
    }

    return result;
}
