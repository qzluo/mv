#ifndef QUPDATEALGPARASONNETWORKDLG_H
#define QUPDATEALGPARASONNETWORKDLG_H

#include <QtWidgets>
#include "qzaoinspectalgparas.h"

class QUpdateAlgParasOnNetworkDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QUpdateAlgParasOnNetworkDlg(QWidget *parent = 0);

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

    QSpinBox* iMinSB;
    QSpinBox* iMaxSB;
    QSpinBox* bMinSB;
    QSpinBox* bMaxSB;
    QSpinBox* rMinSB;
    QSpinBox* rMaxSB;

    QSpinBox* wMinSB;
    QSpinBox* hMinSB;
    QDoubleSpinBox* ratioMaxSB;

    //旧参数值
    QLabel* old_class1LengthDSB;
    QLabel* old_class1WidthDSB;
    QLabel* old_class2LengthDSB;
    QLabel* old_class2WidthDSB;
    QLabel* old_class3LengthDSB;
    QLabel* old_class3WidthDSB;

    QLabel* old_iMinSB;
    QLabel* old_iMaxSB;
    QLabel* old_bMinSB;
    QLabel* old_bMaxSB;
    QLabel* old_rMinSB;
    QLabel* old_rMaxSB;

    QLabel* old_wMinSB;
    QLabel* old_hMinSB;
    QLabel* old_ratioMaxSB;

    double old_class1Length;
    double old_class1Width;
    double old_class2Length;
    double old_class2Width;
    double old_class3Length;
    double old_class3Width;
    int old_iMin;
    int old_iMax;
    int old_bMin;
    int old_bMax;
    int old_rMin;
    int old_rMax;
    int old_wMin;
    int old_hMin;
    double old_ratioMax;
};

#endif // QUPDATEALGPARASONNETWORKDLG_H
