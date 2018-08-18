#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QtSerialPort/QSerialPort>

class SerialComm
{
public:
    SerialComm();
    ~SerialComm();

    int init(QString portName);
    int init(QString portName, int baud, int databit,
             int stopbit, int parity, int timeout=10);

    bool isOpen();
    void clear();
    void close();

    int read(char *data, qint64 maxSize);
    int write(const char *data, qint64 maxSize);

private:
    int setParams(int baud, int databit, int stopbit, int parity);
    int readWithTimeout(char *data, qint64 maxSize, int tout);

private:
    QSerialPort* serialPort;
    int timeout;

};

#endif // SERIALCOMM_H
