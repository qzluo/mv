#ifndef QCTRLBOARDPARASSETUPDLG_H
#define QCTRLBOARDPARASSETUPDLG_H

#include <QtWidgets>
#include "qrwcommcontroller.h"

class QCtrlBoardParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QCtrlBoardParasSetupDlg(QWidget *parent = 0);

    QRWCommController *getRwCommInst() const;
    void setRwCommInst(QRWCommController *value);

signals:

public slots:
    void onReadRegisterBtnClicked();
    void onValveTimeReadBtnClicked();
    void onValveTimeWriteBtnClicked();

    void onValveActionBtnClicked();
    void onSaveValveEnableBtnClicked();

private:
    QRWCommController* rwCommInst;

    QSpinBox* valveNoSP;

    QDoubleSpinBox* valveOpenTimeSB;
    QSpinBox* valveDelaySBs[8];
    QLineEdit* registerConts[8];
};

#endif // QCTRLBOARDPARASSETUPDLG_H
