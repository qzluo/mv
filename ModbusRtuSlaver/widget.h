#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSignalMapper>
#include <QtWidgets>
#include "commcontroller.h"

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
    void OnHasData(QString input);
    void onSigDbRegChanged(int reg_addr, unsigned short value);
    void onSigDbFrameIdChanged(int reg_addr, unsigned int frameId);

private slots:
    void OnSetBtnClicked(bool clicked);

    void onLeftResultEditingFinished();
    void onRightResultEditingFinished();

    void onSbSftFrameIdChange(int i);
    void onSbKzbFrameIdChange(int i);

    void onReadRegBtnClicked();
    void onWriteRegBtnClicked();




signals:
    void setComm(QString comName);
    void sigRegDataSet(int reg_addr, unsigned short value);


private:
    Ui::Widget *ui;
    CommController commController;

    QLineEdit* lineEditComm;
    QLineEdit* leLeftResult;
    QLineEdit* leRightResult;
    QSpinBox* sb_sft_frame_id;
    QSpinBox* sb_kzb_frame_id;

    QSpinBox* sb_reg_addr;
    QLineEdit* le_reg_value;

    QTextEdit* inputText;
    QPushButton* clearBtn;

};

#endif // WIDGET_H
