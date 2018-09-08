#include "qcommsetupdlg.h"

#include <QSerialPort>
#include <QSerialPortInfo>

QCommSetupDlg::QCommSetupDlg(QWidget *parent) : QDialog(parent)
{
    portCB = new QComboBox(this);
    QList<QSerialPortInfo> qPortList = QSerialPortInfo::availablePorts();
    int comCount = qPortList.count();
    for (int iPort = 0; iPort < comCount; iPort++)
        portCB->addItem(qPortList.at(iPort).portName());

    baudCB = new QComboBox(this);
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud1200));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud2400));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud4800));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud9600));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud19200));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud38400));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud57600));
    baudCB->addItem(QString("%1").arg(QSerialPort::Baud115200));

    QFormLayout* portLayout = new QFormLayout;
    portLayout->addRow(tr("Port: "), portCB);
    portLayout->addRow(tr("Baud: "), baudCB);

    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);

    connect(okBtn, &QPushButton::clicked,
            this, &QCommSetupDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QCommSetupDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout;

    topLayout->addLayout(portLayout);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Serial Port Setup"));
}

void QCommSetupDlg::onOkBtnClicked()
{
    QString portName = pSysInfo->getPortName();
    int portBaud = pSysInfo->getPortBaud();
    bool changed = false;

    if (portName.compare(portCB->currentText(), Qt::CaseInsensitive)) {
        pSysInfo->setPortName(portCB->currentText());
        changed = true;
    }

    if (portBaud != baudCB->currentText().toInt()) {
        pSysInfo->setPortBaud(baudCB->currentText().toInt());
        changed = true;
    }

    if (changed)
        pSysInfo->save();

    accept();
}

ProjectSysInfo *QCommSetupDlg::getPSysInfo() const
{
    return pSysInfo;
}

void QCommSetupDlg::setPSysInfo(ProjectSysInfo *value)
{
    pSysInfo = value;
    QString portName = pSysInfo->getPortName();

    int portNameId = portCB->findText(portName, Qt::MatchFixedString);
    if (portNameId >= 0)
        portCB->setCurrentIndex(portNameId);

    int portBaud = pSysInfo->getPortBaud();
    int portBaudId = baudCB->findText(QString::number(portBaud));
    if (portBaudId >= 0)
        baudCB->setCurrentIndex(portBaudId);
}
