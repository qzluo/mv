#include "widget.h"
#include "ui_widget.h"

#include <QJsonDocument>
#include <QJsonObject>


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

    QJsonObject json;

    json.insert("class_good1_length", 1);
    json.insert("class_good1_width", 1);
    json.insert("class_good2_length", 1);
    json.insert("class_good2_width", 1);
    json.insert("class_good3_length", 1);
    json.insert("class_good3_width", 1);

    json.insert("Imin", 1);
    json.insert("Bmin", 1);
    json.insert("Rmin", 1);
    json.insert("Imax", 1);
    json.insert("Bmax", 1);
    json.insert("Rmax", 1);

    json.insert("Wmin", 1);
    json.insert("Hmin", 1);
    json.insert("RatioMax", 1);

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);

    writeUdpDatagram(byte_array.data(), byte_array.size(),
                     ipAddr->text().toLatin1().data(), portSB->value());

//    QString text = sendText->toPlainText();
//    writeUdpDatagram(text.toLatin1().data(), text.size(),
//                     ipAddr->text().toLatin1().data(), portSB->value());
}

void Widget::onReceiveDatagrams(const QByteArray &datagram)
{
    receiveText->setText(datagram.data());

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(datagram, &json_error);
    if (json_error.error != QJsonParseError::NoError)
        return;

    if (!parse_doucment.isObject())
        return;

    QJsonObject obj = parse_doucment.object();
    if (obj.contains("name")) {
        QJsonValue name_value = obj.take("name");
        if(name_value.isString()) {
            QString name = name_value.toString();
        }
    }
}
