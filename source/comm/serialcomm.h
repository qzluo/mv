#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QtSerialPort/QSerialPort>

/*
 * Usage:
 *   1. init(...) to initial serial port.
 *   2. read(...) and write(...) to communicate with serial port.
 *   3. clear(...) is called to clear the read buffer.
 * */
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
    void setParams(int baud, int databit, int stopbit, int parity);
    int readWithTimeout(char *data, qint64 maxSize, int tout);

private:
    QSerialPort* serialPort;
    int timeout;

};

#endif // SERIALCOMM_H
