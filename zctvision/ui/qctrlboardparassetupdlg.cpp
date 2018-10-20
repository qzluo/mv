#include "qctrlboardparassetupdlg.h"

QCtrlBoardParasSetupDlg::QCtrlBoardParasSetupDlg(QWidget *parent) : QDialog(parent)
{
    rwCommInst = NULL;

    //valve enable
    QGroupBox* valveEnableGB = new QGroupBox(tr("Valve Test"));
    QHBoxLayout* valveEnableLayout = new QHBoxLayout(valveEnableGB);

//    QCheckBox* valveEnableCB = new QCheckBox(tr("Enable"));
    valveNoSP = new QSpinBox;
    valveNoSP->setRange(1, 8);
    valveNoSP->setValue(1);
    QPushButton* valveActionPB = new QPushButton(tr("Valve Action"));
    QPushButton* saveValveEnablePB = new QPushButton(tr("Save Setup"));
    connect(valveActionPB, SIGNAL(clicked(bool)),
            this, SLOT(onValveActionBtnClicked()));
    connect(saveValveEnablePB, SIGNAL(clicked(bool)),
            this, SLOT(onSaveValveEnableBtnClicked()));

//    valveEnableLayout->addWidget(valveEnableCB);
//    valveEnableLayout->addStretch();
    valveEnableLayout->addWidget(new QLabel("Select Valve"));
    valveEnableLayout->addWidget(valveNoSP);
    valveEnableLayout->addStretch();
    valveEnableLayout->addWidget(valveActionPB);
    valveEnableLayout->addWidget(saveValveEnablePB);

    //valve time
    QHBoxLayout* valveOpenTimeLayout = new QHBoxLayout;
    valveOpenTimeLayout->addWidget(new QLabel(tr("Valve Open Time")));
    valveOpenTimeSB = new QDoubleSpinBox;
    valveOpenTimeLayout->addWidget(valveOpenTimeSB);
    valveOpenTimeLayout->addWidget(new QLabel("ms"));
    valveOpenTimeLayout->addStretch();


    QFormLayout* valveDelayLeftLayout = new QFormLayout;
    QFormLayout* valveDelayRightLayout = new QFormLayout;

    for (int i = 0; i < 8; ++i) {
        valveDelaySBs[i] = new QSpinBox();
        valveDelaySBs[i]->setRange(0, 10000);
        if (i % 2 == 0)
            valveDelayLeftLayout->addRow(QString(tr("Valve%1 Delay")).arg(i+1),
                                         valveDelaySBs[i]);
        else
            valveDelayRightLayout->addRow(QString(tr("Valve%1 Delay")).arg(i+1),
                                          valveDelaySBs[i]);
    }

    QHBoxLayout* valveDelayLayout = new QHBoxLayout;
    valveDelayLayout->addLayout(valveDelayLeftLayout);
    valveDelayLayout->addSpacing(10);
    valveDelayLayout->addLayout(valveDelayRightLayout);

    QPushButton* valveTimeReadBtn = new QPushButton(tr("Read"));
    QPushButton* valveTimeWriteBtn = new QPushButton(tr("Write"));
    connect(valveTimeReadBtn, SIGNAL(clicked(bool)),
            this, SLOT(onValveTimeReadBtnClicked()));
    connect(valveTimeWriteBtn, SIGNAL(clicked(bool)),
            this, SLOT(onValveTimeWriteBtnClicked()));

    QHBoxLayout* valveTimeBtnLayout = new QHBoxLayout;
    valveTimeBtnLayout->addStretch();
    valveTimeBtnLayout->addWidget(valveTimeReadBtn);
    valveTimeBtnLayout->addWidget(valveTimeWriteBtn);

    QGroupBox* valveTimeGB = new QGroupBox(tr("Valve Time"));
    QVBoxLayout* valveLayout = new QVBoxLayout(valveTimeGB);
    valveLayout->addLayout(valveOpenTimeLayout);
    valveLayout->addLayout(valveDelayLayout);
    valveLayout->addLayout(valveTimeBtnLayout);

    //register
    QFormLayout* registerContsLeftLayout = new QFormLayout;
    QFormLayout* registerContsRightLayout = new QFormLayout;
    QLabel* regiserLabels[8] = {};
    for (int i = 0; i < 8; ++i) {
        regiserLabels[i] = new QLabel(QString(tr("Register#%1")).arg(i+1));
        registerConts[i] = new QLineEdit();
        if (i % 2 == 0)
            registerContsLeftLayout->addRow(regiserLabels[i],
                                         registerConts[i]);
        else
            registerContsRightLayout->addRow(regiserLabels[i],
                                          registerConts[i]);
    }

    QHBoxLayout* registerContsLayout = new QHBoxLayout;
    registerContsLayout->addLayout(registerContsLeftLayout);
    registerContsLayout->addStretch();
    registerContsLayout->addLayout(registerContsRightLayout);

    QPushButton* readRegisterBtn = new QPushButton(tr("Read Controller Registers"));
    connect(readRegisterBtn, SIGNAL(clicked(bool)),
            this, SLOT(onReadRegisterBtnClicked()));

    QHBoxLayout* registerBtnLayout = new QHBoxLayout;
    registerBtnLayout->addStretch();
    registerBtnLayout->addWidget(readRegisterBtn);

    QGroupBox* registerGB = new QGroupBox(tr("Register Contents"));
    QVBoxLayout* registerLayout = new QVBoxLayout(registerGB);
    registerLayout->addLayout(registerContsLayout);
    registerLayout->addLayout(registerBtnLayout);

    QVBoxLayout* topLayout = new QVBoxLayout;

    topLayout->addWidget(valveEnableGB);
    topLayout->addWidget(valveTimeGB);
    topLayout->addWidget(registerGB);

    setLayout(topLayout);
    setWindowTitle(tr("Controller Board Parameters"));
}

QRWCommController *QCtrlBoardParasSetupDlg::getRwCommInst() const
{
    return rwCommInst;
}

void QCtrlBoardParasSetupDlg::setRwCommInst(QRWCommController *value)
{
    rwCommInst = value;

    if (!rwCommInst)
        return;

    onReadRegisterBtnClicked();
    onValveTimeReadBtnClicked();

    //初始化气阀选择
    unsigned short data = 0;
    rwCommInst->readRegisters(19, 1, &data);
    if (data <= 8)
        valveNoSP->setValue(data);
}

void QCtrlBoardParasSetupDlg::onReadRegisterBtnClicked()
{
    if (!rwCommInst)
        return;

    unsigned short data[8] = {};
    if (rwCommInst->readRegisters(1, 8, data) != 0)
        return;

    for (int i = 0; i < 8; i++) {
        registerConts[i]->setText(QString("0x%1").arg(data[i] & 0xFFFF, 0, 16));
    }
}

void QCtrlBoardParasSetupDlg::onValveTimeReadBtnClicked()
{
    if (!rwCommInst)
        return;

    unsigned short data[9] = {};
    if (rwCommInst->readRegisters(33, 9, data) != 0)
        return;

    valveOpenTimeSB->setValue(data[0] / 10.0);
    for (int i = 0; i < 8; i++) {
        valveDelaySBs[i]->setValue(data[i+1]);
    }
}

void QCtrlBoardParasSetupDlg::onValveTimeWriteBtnClicked()
{
    if (!rwCommInst)
        return;

    unsigned short data[9] = {};

    data[0] = unsigned short(valveOpenTimeSB->value() * 10);
    for (int i = 0; i < 8; ++i)
        data[i+1] = valveDelaySBs[i]->value();

    rwCommInst->writeRegisters(33, 9, data);
}

void QCtrlBoardParasSetupDlg::onValveActionBtnClicked()
{
    if (!rwCommInst)
        return;

    unsigned short data = 0;
    data = valveNoSP->value();

    if (rwCommInst->writeRegister(19, data) != 0)
        return;

    //气阀动作，上升沿有效
    if (rwCommInst->readRegisters(20, 1, &data) != 0)
        return;

    if (data & 0x01) {
        data &= 0xFFFE;

        //置0
        if (rwCommInst->writeRegister(20, data) != 0)
            return;

        //延时
        QThread::msleep(100);
    }

    //置1
    data |= 0x01;
    rwCommInst->writeRegister(20, data);
}

void QCtrlBoardParasSetupDlg::onSaveValveEnableBtnClicked()
{
    if (!rwCommInst)
        return;

    unsigned short data = (unsigned short)(valveNoSP->value());

    rwCommInst->writeRegister(19, data);
}
