#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "qudpserver.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void onStartSrvBtnClicked(void);
    void onStopSrvBtnClicked(void);
    void onSendBtnClicked(void);

    void onReceiveDatagrams(const QByteArray& datagram);

private:
    Ui::Widget *ui;

    QUdpServer udpServer;

    QPushButton* startSrvBtn;
    QTextEdit* receiveText;
    QTextEdit* sendText;
    QLineEdit* ipAddr;
    QSpinBox* portSB;
};

#endif // WIDGET_H
