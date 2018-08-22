#ifndef QRTUOPERATORDLG_H
#define QRTUOPERATORDLG_H

#include <QtWidgets>

#include "qrwcommcontroller.h"

class QRtuOperatorDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QRtuOperatorDlg(QWidget *parent = 0);

    QRWCommController *getRwCommInst() const;
    void setRwCommInst(QRWCommController *value);

signals:

public slots:
    void onReadBtnClicked();
    void onWriteBtnClicked();

private:
    QRWCommController* rwCommInst;

    QSpinBox* regAddrSB;
    QLineEdit* regValueLE;
};

#endif // QRTUOPERATORDLG_H
