#include "serialcomm.h"

SerialComm::SerialComm()
{
    serialPort = NULL;
}

SerialComm::~SerialComm()
{
    close();
}

int SerialComm::init(QString portName)
{
    return init(portName, 9600, 8, 1, 0, 150);
}

/*
 * Init a serial port by open it.
 * return: 0  -- open success
 *         -1 -- failed
 *
 * */
int SerialComm::init(QString portName, int baud, int databit, int stopbit, int parity, int timeout)
{    
    if (baud != 1200 && baud != 2400 && baud != 4800 &&
            baud != 9600 && baud != 19200 && baud != 38400 &&
            baud != 57600 && baud != 115200)
        return -1;

    if (databit < QSerialPort::Data5 || databit > QSerialPort::Data8)
        return -1;

    if (stopbit < QSerialPort::OneStop || stopbit > QSerialPort::OneAndHalfStop)
        return -1;

    if (parity < QSerialPort::NoParity || parity > QSerialPort::MarkParity)
        return -1;

    close();

    serialPort = new QSerialPort(portName);
    if (!serialPort)
        return -1;

    setParams(baud, databit, stopbit, parity);
    this->timeout = timeout;

    if (!serialPort->open(QIODevice::ReadWrite)) {
        delete serialPort;
        serialPort = NULL;
        return -1;
    }

    return 0;
}

bool SerialComm::isOpen()
{
    return serialPort->isOpen();
}

void SerialComm::clear()
{
    serialPort->clear();
}

void SerialComm::close()
{
    if (serialPort) {
        if (isOpen())
            serialPort->close();

        delete serialPort;
        serialPort = NULL;
    }
}

int SerialComm::read(char *data, qint64 maxSize)
{
    return readWithTimeout(data, maxSize, timeout);
}

/*
 * data: buffer
 * maxSize: max lenth to write
 * return: >=0  -- data length write
 *         -1 -- failed
 *
 * */
int SerialComm::write(const char *data, qint64 maxSize)
{
    return serialPort->write(data, maxSize);
}

void SerialComm::setParams(int baud, int databit, int stopbit, int parity)
{
    if (!serialPort)
        return ;

    serialPort->setBaudRate(baud);
    serialPort->setDataBits((enum QSerialPort::DataBits)databit);
    serialPort->setStopBits((enum QSerialPort::StopBits)stopbit);
    serialPort->setParity((enum QSerialPort::Parity)parity);
}

/*
 * data: buffer
 * maxSize: max lenth to read
 * tout: timeout
 * return: >=0  -- data length read
 *         -1 -- failed
 *
 * */
int SerialComm::readWithTimeout(char *data, qint64 maxSize, int tout)
{
    qint64 totalCounts = 0;
    qint64 readCounts = 0;
    char* pos = data;

    readCounts = serialPort->read(pos, maxSize);
    if (readCounts > 0) {
        totalCounts += readCounts;
        maxSize -= readCounts;
        pos += readCounts;
    }

    if (maxSize <= 0)
        return totalCounts;

    while (serialPort->waitForReadyRead(tout)) {
        readCounts = serialPort->read(pos, maxSize);
        if (readCounts > 0) {
            totalCounts += readCounts;
            maxSize -= readCounts;
            pos += readCounts;
        }

        if (maxSize <= 0)
            break;
    }

    if (serialPort->error() == QSerialPort::ReadError)
        return -1;
    else if (serialPort->error() == QSerialPort::TimeoutError && pos == data)
        return 0;

    return totalCounts;
}
