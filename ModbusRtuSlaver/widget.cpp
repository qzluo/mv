#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QGroupBox>

#include <QLabel>


#include "widget.h"
#include "ui_widget.h"
#include "public.h"

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QLabel* labelComm = new QLabel("Com: ", this);
    lineEditComm = new QLineEdit(this);
    lineEditComm->setText("Com2");
    QPushButton* setComm = new QPushButton("Reset", this);
    connect(setComm, SIGNAL(clicked(bool)), this, SLOT(OnSetBtnClicked(bool)));

    QHBoxLayout* commLayout = new QHBoxLayout();
    commLayout->addWidget(labelComm);
    commLayout->addWidget(lineEditComm);
    commLayout->addWidget(setComm);

    //class result
    QLabel* leftResult = new QLabel("Left Result: ", this);
    leLeftResult = new QLineEdit(this);
    QLabel* rightResult = new QLabel("Right Result: ", this);
    leRightResult = new QLineEdit(this);

    connect(leLeftResult, &QLineEdit::editingFinished,
            this, &Widget::onLeftResultEditingFinished);
    connect(leRightResult, &QLineEdit::editingFinished,
            this, &Widget::onRightResultEditingFinished);

    QHBoxLayout* resultLayout = new QHBoxLayout();
    resultLayout->addWidget(leftResult);
    resultLayout->addWidget(leLeftResult);
    resultLayout->addStretch();
    resultLayout->addWidget(rightResult);
    resultLayout->addWidget(leRightResult);

    //frame id
    QLabel* label_sft_frame_id = new QLabel("sft_frame_id: ", this);
    sb_sft_frame_id = new QSpinBox(this);
    QLabel* label_kzb_frame_id = new QLabel("kzb_frame_id: ", this);
    sb_kzb_frame_id = new QSpinBox(this);
    sb_sft_frame_id->setRange(0, 0xFFFF);
    sb_kzb_frame_id->setRange(0, 0xFFFF);

    connect(sb_sft_frame_id, SIGNAL(valueChanged(int)),
            this, SLOT(onSbSftFrameIdChange(int)));
    connect(sb_kzb_frame_id, SIGNAL(valueChanged(int)),
            this, SLOT(onSbKzbFrameIdChange(int)));

    QHBoxLayout* frameIdLayout = new QHBoxLayout();
    frameIdLayout->addWidget(label_sft_frame_id);
    frameIdLayout->addWidget(sb_sft_frame_id);
    frameIdLayout->addStretch();
    frameIdLayout->addWidget(label_kzb_frame_id);
    frameIdLayout->addWidget(sb_kzb_frame_id);

    //any addr
    QGroupBox* regGB = new QGroupBox(this);
    QLabel* lb_reg_addr = new QLabel("reg addr: ");
    sb_reg_addr = new QSpinBox();

    QLabel* lb_reg_value = new QLabel("value: ");
    le_reg_value = new QLineEdit();

    QPushButton* readBtn = new QPushButton("Read");
    QPushButton* writeBtn = new QPushButton("Write");

    connect(readBtn, SIGNAL(clicked(bool)), this, SLOT(onReadRegBtnClicked()));
    connect(writeBtn, SIGNAL(clicked(bool)), this, SLOT(onWriteRegBtnClicked()));

    QHBoxLayout* regLayout = new QHBoxLayout();
    regLayout->addWidget(lb_reg_addr);
    regLayout->addWidget(sb_reg_addr);
    regLayout->addStretch();
    regLayout->addWidget(lb_reg_value);
    regLayout->addWidget(le_reg_value);
    regLayout->addStretch();
    regLayout->addWidget(readBtn);
    regLayout->addWidget(writeBtn);

    regGB->setLayout(regLayout);

    inputText = new QTextEdit(this);
    inputText->setMinimumSize(100, 100);

    clearBtn = new QPushButton("clear", this);
    connect(clearBtn, SIGNAL(clicked(bool)), inputText, SLOT(clear()));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(10);

    layout->addLayout(commLayout);
    layout->addLayout(resultLayout);
    layout->addLayout(frameIdLayout);
    layout->addWidget(regGB);

    layout->addWidget(inputText);
    layout->addWidget(clearBtn);
    setLayout(layout);

    connect(&commController, SIGNAL(hasData(QString)),
            this, SLOT(OnHasData(QString)));
//    connect(&commController, SIGNAL(hasWriteData(int)), this, SLOT(OnHasWriteData(int)));

    connect(this, SIGNAL(setComm(QString)),
            &commController, SIGNAL(commNameChanged(QString)));

    connect(this, &Widget::sigRegDataSet,
            &commController, &CommController::onSigRegDataSet);

    connect(&commController, &CommController::sigDbRegChanged,
            this, &Widget::onSigDbRegChanged);

    connect(&commController, &CommController::sigDbFrameIdChanged,
            this, &Widget::onSigDbFrameIdChanged);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::OnHasData(QString input)
{
    inputText->append(input);
}

void Widget::onSigDbRegChanged(int reg_addr, unsigned short value)
{
    if (reg_addr == 1) {
        leLeftResult->setText(QString("0x%1").arg(value & 0xFFFF, 0, 16));
    }
    else if (reg_addr == 2) {
        leRightResult->setText(QString("0x%1").arg(value & 0xFFFF, 0, 16));
    }
}

void Widget::onSigDbFrameIdChanged(int reg_addr, unsigned int frameId)
{
    if (reg_addr == 3) {
        sb_sft_frame_id->setValue(frameId);
    }
    else if (reg_addr == 5) {
        sb_kzb_frame_id->setValue(frameId);
    }
}

void Widget::OnSetBtnClicked(bool clicked)
{
    emit setComm(lineEditComm->text());
}

void Widget::onLeftResultEditingFinished()
{
    emit sigRegDataSet(1, leLeftResult->text().toUShort(NULL, 16));
}

void Widget::onRightResultEditingFinished()
{
    emit sigRegDataSet(2, leRightResult->text().toUShort(NULL, 16));
}

void Widget::onSbSftFrameIdChange(int value)
{
    emit sigRegDataSet(3, value & 0xFFFF);
    emit sigRegDataSet(4, (value >> 16) & 0xFFFF);
}

void Widget::onSbKzbFrameIdChange(int value)
{
    emit sigRegDataSet(5, value & 0xFFFF);
    emit sigRegDataSet(6, (value >> 16) & 0xFFFF);
}

void Widget::onReadRegBtnClicked()
{
    int reg_addr = sb_reg_addr->value();
    unsigned short value = 0;
    if (commController.readRegAdd(reg_addr, &value) == 0)
        le_reg_value->setText(QString("0x%1").arg(value & 0xFFFF, 0, 16));
}

void Widget::onWriteRegBtnClicked()
{
    int reg_addr = sb_reg_addr->value();
    emit sigRegDataSet(reg_addr, le_reg_value->text().toUShort(NULL, 16));
}

