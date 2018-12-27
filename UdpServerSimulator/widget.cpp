#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    startSrvBtn = new QPushButton("Start Server", this);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(startSrvBtn);

    connect(startSrvBtn, &QPushButton::clicked,
            this, &Widget::onStartSrvBtnClicked);

    //receive text
    QGroupBox* receiveGB = new QGroupBox("Receive Data");
    QVBoxLayout* receiveLayout = new QVBoxLayout(receiveGB);
    receiveText = new QTextEdit(this);
    receiveText->setMinimumSize(100, 100);

    receiveLayout->addWidget(receiveText);

    //send text
    QGroupBox* sendGB = new QGroupBox("Send Data");
    QVBoxLayout* sendLayout = new QVBoxLayout(sendGB);
    sendText = new QTextEdit(this);
    sendText->setMinimumSize(100, 100);

    ipAddr = new QLineEdit();
    portSB = new QSpinBox();
    portSB->setRange(1000, 100000);

    QPushButton* sendBtn = new QPushButton("Send");
    connect(sendBtn, &QPushButton::clicked,
            this, &Widget::onSendBtnClicked);

    QHBoxLayout* sendCmdLayout = new QHBoxLayout;
    sendCmdLayout->addWidget(new QLabel("Ip address:"));
    sendCmdLayout->addWidget(ipAddr);
    sendCmdLayout->addWidget(new QLabel("Port:"));
    sendCmdLayout->addWidget(portSB);
    sendCmdLayout->addStretch();
    sendCmdLayout->addWidget(sendBtn);

    sendLayout->addWidget(sendText);
    sendLayout->addLayout(sendCmdLayout);

    QVBoxLayout* topLayout = new QVBoxLayout;
    topLayout->addLayout(btnLayout);
    topLayout->addWidget(receiveGB);
    topLayout->addWidget(sendGB);

    setLayout(topLayout);

    connect(&udpServer, &QUdpServer::receiveDatagrams,
            this, &Widget::onReceiveDatagrams);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onStartSrvBtnClicked()
{
    if (startSrvBtn->text() == "Start Server") {
        if (udpServer.initSocket() == 0) {
            startSrvBtn->setText("Stop Server");
        }
    }
    else {
        udpServer.closeSocket();
        startSrvBtn->setText("Start Server");
    }
}

void Widget::onStopSrvBtnClicked()
{
    udpServer.closeSocket();
}

void Widget::onSendBtnClicked()
{
    QString text = sendText->toPlainText();
    writeUdpDatagram(text.toLatin1().data(), text.size(),
                     ipAddr->text().toLatin1().data(), portSB->value());
}

void Widget::onReceiveDatagrams(const QByteArray &datagram)
{
    receiveText->setText(datagram.data());
}
