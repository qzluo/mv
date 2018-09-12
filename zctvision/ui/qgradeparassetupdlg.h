#ifndef QGRADEPARASSETUPDLG_H
#define QGRADEPARASSETUPDLG_H

#include <QtWidgets>
#include <QWidget>
#include "qzaoinspectalgparas.h"

class QGradeParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QGradeParasSetupDlg(QWidget *parent = 0);

    QZaoInspectAlgParas *getPInspectAlgParas() const;
    void setPInspectAlgParas(QZaoInspectAlgParas *value);

private:
    void updateInspectParas(QZaoInspectAlgParas* pInspectAlgParas);

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

#endif // QGRADEPARASSETUPDLG_H
