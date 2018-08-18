#ifndef CMODBUSRTUTESTER_H
#define CMODBUSRTUTESTER_H

#include <QObject>
#include "cmodbusrtu.h"


class CModbusRtuTester : public QObject
{
    Q_OBJECT
public:
    explicit CModbusRtuTester(QObject *parent = 0);

    int init();
    void testreadRegisters();
    void testwriteRegister();
    void testwriteRegisters();

signals:

public slots:

private:
    CModBusRtu rtu;
    SerialComm serialcomm;

};

#endif // CMODBUSRTUTESTER_H
