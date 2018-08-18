#ifndef COMMCONTROLLER_H
#define COMMCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "serialcomm.h"

#define COMMWORKER_DELAY_TIME  10

#define BYTESTOSHORT(hbyte,lbyte)   ((short(hbyte)<<8)|(short(lbyte)&0xFF))
#define HIGHBYTE(w)                 ((w>>8)&0xFF)
#define LOWBYTE(w)                  (w&0xFF)

typedef struct YcData {
    int no;
    char addr[8];
    int value;
} YCDATA, *PYCDATA;

class CommWorker : public QObject
{
    Q_OBJECT

public:
    CommWorker(QObject *parent = 0);

    void setRegData(int reg_addr, unsigned short value);
    int readRegAdd(int reg_addr, unsigned short* value);

protected:
    void timerEvent(QTimerEvent *event);

private:
    void handleInput(char *input, int dataLen);

    int handleReadRegister(char* input, int dataLen, char* output, int* outLen);
    int handleWriteRegister(char* input, int dataLen, char* output, int* outLen);
    int handleWriteRegisters(char* input, int dataLen, char* output, int* outLen);

signals:
    void hasCommInput(QString);
    void sigDbRegChanged(int reg_addr, unsigned short value);
    void sigDbFrameIdChanged(int reg_addr, unsigned int frameId);

public slots:
    void init(void);
    void deinit(void);
    void onCommNameChanged(QString comName);

private:
    int readTimerId;
    SerialComm* serialComm;
    QMutex mutex;
};


class CommController : public QObject
{
    Q_OBJECT

public:
    explicit CommController(QObject *parent = 0);
    ~CommController();

    int readRegAdd(int reg_addr, unsigned short* value);

public slots:
    void onSigRegDataSet(int reg_addr, unsigned short value);

signals:
    void hasData(QString input);

    void commNameChanged(QString comName);

    void sigDbRegChanged(int reg_addr, unsigned short value);
    void sigDbFrameIdChanged(int reg_addr, unsigned int frameId);

private:
    QThread workerThread;
    CommWorker worker;
};


#endif // COMMCONTROLLER_H
