#ifndef QALGPARASSETUPDLG_H
#define QALGPARASSETUPDLG_H

#include <QDialog>
#include <QtWidgets>

#include "qzaoinspectalgparas.h"
#include "graphicswidget.h"
#include "qzaoinspectalgapp.h"

class QAlgParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QAlgParasSetupDlg(QWidget *parent = 0);

    QZaoInspectAlgParas *getPInspectAlgParas() const;
    void setPInspectAlgParas(QZaoInspectAlgParas *value);

private:
    void updateInspectParas(QZaoInspectAlgParas* pInspectAlgParas);
    QString getIdDesc(int classId);

signals:

public slots:
    void onSelImgBtnClicked();
    void onTestBtnClicked();
    void onOkBtnClicked();

private:
    QImage image;
    GraphicsWidget* plot;
    QZaoInspectAlgParas* pInspectAlgParas;
    QZaoInspectAlgApp zaoInspectAlg;

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

};

#endif // QALGPARASSETUPDLG_H
