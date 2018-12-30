#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>

#define DEFAULT_DATAUPDATE_PORT 7755

int writeUdpDatagram(const char *data, qint64 size,
                     const char *address, quint16 port);

class QUdpClientBroadcast
{
public:
    QUdpClientBroadcast(void);

    QString getNetworkSegment() const;
    void setNetworkSegment(const QString &value);

    int broadcastUdpDatagram(const char *data, qint64 size);

private:
    QString networkSegment;
    int port;
};

class QUdpServer : public QObject
{
    Q_OBJECT
public:
    explicit QUdpServer(QObject *parent = 0);
    ~QUdpServer();

    int initSocket();
    void closeSocket();

    QString getNetworkSegment() const;
    void setNetworkSegment(const QString &value);

private:
    bool isIpAddrHost(QHostAddress& sender); //判断一个地址是否是本机地址

signals:
    void receiveDatagrams(const QByteArray&);

public slots:
    void readPendingDatagrams();

private:
    QUdpSocket* udpSocket;
    QString networkSegment;
};

#endif // QUDPSERVER_H
