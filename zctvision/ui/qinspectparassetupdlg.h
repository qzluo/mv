#ifndef QINSPECTPARASSETUPDLG_H
#define QINSPECTPARASSETUPDLG_H

#include <QtWidgets>
#include "qzaoinspectalgparas.h"

class QInspectParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QInspectParasSetupDlg(QWidget *parent = 0);

    QZaoInspectAlgParas *getPInspectAlgParas() const;
    void setPInspectAlgParas(QZaoInspectAlgParas *value);

private:
    void updateInspectParas(QZaoInspectAlgParas* pInspectAlgParas);

public slots:
    void onOkBtnClicked();

private:
    QZaoInspectAlgParas* pInspectAlgParas;

    QSpinBox* iMinSB;
    QSpinBox* iMaxSB;
    QSpinBox* bMinSB;
    QSpinBox* bMaxSB;
    QSpinBox* rMinSB;
    QSpinBox* rMaxSB;

    QSpinBox* wMinSB;
    QSpinBox* hMinSB;
    QDoubleSpinBox* ratioMaxSB;
};

#endif // QINSPECTPARASSETUPDLG_H
