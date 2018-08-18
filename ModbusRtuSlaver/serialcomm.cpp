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
    return init(portName, 57600, 8, 1, 0, 10);
}

int SerialComm::init(QString portName, int baud, int databit, int stopbit, int parity, int timeout)
{
    close();

    serialPort = new QSerialPort(portName);
    if (!serialPort)
        return -1;

    if (setParams(baud, databit, stopbit, parity) < 0) {
        delete serialPort;
        serialPort = NULL;
        return -1;
    }

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
    if (!serialPort)
        return false;

    return serialPort->isOpen();
}

void SerialComm::clear()
{
    char tmpBuffer[1024] = {};
    while (readWithTimeout(tmpBuffer, 1024, 1) == 1024)
        ;
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
 * maxSize: max lenth to read
 * return: >=0  -- data length read
 *         -1 -- failed
 *
 * */
int SerialComm::write(const char *data, qint64 maxSize)
{
    return serialPort->write(data, maxSize);
}

int SerialComm::setParams(int baud, int databit, int stopbit, int parity)
{
    if (!serialPort)
        return -1;

    if (!serialPort->setBaudRate(baud))
        return -1;

    if (!serialPort->setDataBits((enum QSerialPort::DataBits)databit))
        return -1;

    if (!serialPort->setStopBits((enum QSerialPort::StopBits)stopbit))
        return -1;

    if (!serialPort->setParity((enum QSerialPort::Parity)parity))
        return -1;

    return serialPort->error() == QSerialPort::NoError ? 0 : -1;
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
