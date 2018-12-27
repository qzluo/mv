#include "qudpserver.h"
#include <qnetworkinterface.h>
#include <QDebug>

QUdpServer::QUdpServer(QObject *parent) : QObject(parent)
{
    udpSocket = NULL;

    networkSegment = "192.168.0.";
}

QUdpServer::~QUdpServer()
{
    closeSocket();
}

int QUdpServer::initSocket()
{
    closeSocket();

    udpSocket = new QUdpSocket(this);

    //get host ip
    QList<QHostAddress> listAddress = QNetworkInterface::allAddresses();
    QHostAddress hostAddr;
    bool isIpFinded = false;
    for (int i = 0; i < listAddress.size(); ++i) {
        QHostAddress address = listAddress.at(i);
        QString strIp = address.toString();

        if (strIp.contains(networkSegment)) {
            hostAddr = address;
            isIpFinded = true;
            break;
        }
    }

    if (!isIpFinded)
        return -1;

    qDebug() << hostAddr;

    if (!udpSocket->bind(hostAddr, 7755)) {
        delete udpSocket;
        udpSocket = NULL;
        return -1;
    }

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));

    return 0;
}

void QUdpServer::closeSocket()
{
    if (udpSocket) {
        if (udpSocket->isOpen())
            udpSocket->close();

        delete udpSocket;
        udpSocket = NULL;
    }
}

void QUdpServer::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        emit receiveDatagrams(datagram);
    }
}

QString QUdpServer::getNetworkSegment() const
{
    return networkSegment;
}

void QUdpServer::setNetworkSegment(const QString &value)
{
    networkSegment = value + ".";
}

int writeUdpDatagram(const char *data, qint64 size, const char *address, quint16 port)
{
    QUdpSocket udpClient;
    return udpClient.writeDatagram(data, size, QHostAddress(address), port);
}
