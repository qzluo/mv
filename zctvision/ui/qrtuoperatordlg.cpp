#include "qrtuoperatordlg.h"

QRtuOperatorDlg::QRtuOperatorDlg(QWidget *parent) : QDialog(parent)
{
    rwCommInst = NULL;

    QLabel* regAddrLbl = new QLabel(tr("Register Address: "), this);
    regAddrSB = new QSpinBox(this);
    regAddrSB->setRange(0, 1000);

    QLabel* regValueLbl = new QLabel(tr("Value: "), this);
    regValueLE = new QLineEdit(this);

    QPushButton* readBtn = new QPushButton(tr("Read"), this);
    QPushButton* writeBtn = new QPushButton(tr("Write"), this);

    connect(readBtn, &QPushButton::clicked,
            this, &QRtuOperatorDlg::onReadBtnClicked);
    connect(writeBtn, &QPushButton::clicked,
            this, &QRtuOperatorDlg::onWriteBtnClicked);

    QHBoxLayout* regLayout = new QHBoxLayout;
    regLayout->addWidget(regAddrLbl);
    regLayout->addWidget(regAddrSB);
    regLayout->addStretch();
    regLayout->addWidget(regValueLbl);
    regLayout->addWidget(regValueLE);

    regLayout->addStretch();
    regLayout->addWidget(readBtn);
    regLayout->addWidget(writeBtn);

    setLayout(regLayout);
    setWindowTitle(tr("Modbus Command"));
}

QRWCommController *QRtuOperatorDlg::getRwCommInst() const
{
    return rwCommInst;
}

void QRtuOperatorDlg::setRwCommInst(QRWCommController *value)
{
    rwCommInst = value;
}

void QRtuOperatorDlg::onReadBtnClicked()
{
    if (!rwCommInst)
        return;

    int reg_addr = regAddrSB->value();
    unsigned short value = 0;
    if (rwCommInst->readRegisters(reg_addr, 1, &value) == 0) {
        regValueLE->setText(QString("0x%1").arg(value & 0xFFFF, 0, 16));
    }
}

void QRtuOperatorDlg::onWriteBtnClicked()
{
    if (!rwCommInst)
        return;

    int reg_addr = regAddrSB->value();
    unsigned short value = regValueLE->text().toUShort(NULL, 16);
    if (rwCommInst->writeRegister(reg_addr, value) == 0)
        QMessageBox::information(this, tr("Info"), tr("Write Successfully."));
    else
        QMessageBox::information(this, tr("Info"), tr("Failed to Write!"));
}
