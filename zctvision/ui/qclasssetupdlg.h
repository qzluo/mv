#ifndef QCLASSSETUPDLG_H
#define QCLASSSETUPDLG_H

#include <QDialog>
#include "qzaoinspectalgparas.h"

namespace Ui {
class QClassSetupDlg;
}

class QClassSetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QClassSetupDlg(QWidget *parent = 0);
    ~QClassSetupDlg();

    QZaoInspectAlgParas *getPInspectAlgParas() const;
    void setPInspectAlgParas(QZaoInspectAlgParas *value);

private:
    void updateInspectParas(QZaoInspectAlgParas* pInspectAlgParas);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::QClassSetupDlg *ui;

    QZaoInspectAlgParas* pInspectAlgParas;
};

#endif // QCLASSSETUPDLG_H
