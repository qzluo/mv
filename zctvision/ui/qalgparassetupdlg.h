#ifndef QALGPARASSETUPDLG_H
#define QALGPARASSETUPDLG_H

#include <QDialog>
#include <QtWidgets>

#include "qzaoinspectalgparas.h"

class QAlgParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QAlgParasSetupDlg(QWidget *parent = 0);

    QZaoInspectAlgParas *getPInspectAlgParas() const;
    void setPInspectAlgParas(QZaoInspectAlgParas *value);

signals:

public slots:
    void onOkBtnClicked();

private:
    QZaoInspectAlgParas* pInspectAlgParas;

    QDoubleSpinBox* class1LengthDSB;
    QDoubleSpinBox* class1WidthDSB;
    QDoubleSpinBox* class2LengthDSB;
    QDoubleSpinBox* class2WidthDSB;
    QDoubleSpinBox* class3LengthDSB;
    QDoubleSpinBox* class3WidthDSB;

};

#endif // QALGPARASSETUPDLG_H
