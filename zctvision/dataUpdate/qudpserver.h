#ifndef QUDPSERVER_H
#define QUDPSERVER_H

#include <QObject>
#include <QUdpSocket>

int writeUdpDatagram(const char *data, qint64 size,
                     const char *address, quint16 port);

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

signals:
    void receiveDatagrams(const QByteArray&);

public slots:
    void readPendingDatagrams();

private:
    QUdpSocket* udpSocket;
    QString networkSegment;
};

#endif // QUDPSERVER_H
