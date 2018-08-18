#ifndef QRWCOMMCONTROLLERTESTER_H
#define QRWCOMMCONTROLLERTESTER_H

#include <QObject>
#include "qrwcommcontroller.h"

class QRwCommControllerTester : public QObject
{
    Q_OBJECT
public:
    explicit QRwCommControllerTester(QObject *parent = 0);

    void init();
    void testGetFrameIndex(void);
    void testSetInspectResult(void);

    void testWriteRegister(void);
    void testWriteRegisters(void);

    void testProductClassToData(void);

signals:

public slots:

private:
    unsigned short productClassToData(int productClass);


private:
    QRWCommController controller;
};

#endif // QRWCOMMCONTROLLERTESTER_H
