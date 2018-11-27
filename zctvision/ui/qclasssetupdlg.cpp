#include "qclasssetupdlg.h"
#include "ui_qclasssetupdlg.h"

QClassSetupDlg::QClassSetupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QClassSetupDlg)
{
    ui->setupUi(this);

    setFixedSize(this->width(),this->height());
    setWindowTitle(tr("Class Parameters Setup"));
}

QClassSetupDlg::~QClassSetupDlg()
{
    delete ui;
}

QZaoInspectAlgParas *QClassSetupDlg::getPInspectAlgParas() const
{
    return pInspectAlgParas;
}

void QClassSetupDlg::setPInspectAlgParas(QZaoInspectAlgParas *value)
{
    if (!value)
        return;

    pInspectAlgParas = value;

    ui->class1LengthSB->setValue(int(pInspectAlgParas->getClass_good1_length()));
    ui->class1WidthSB->setValue(int(pInspectAlgParas->getClass_good1_width()));
    ui->class2LengthSB->setValue(int(pInspectAlgParas->getClass_good2_length()));
    ui->class2WidthSB->setValue(int(pInspectAlgParas->getClass_good2_width()));
    ui->class3LengthSB->setValue(int(pInspectAlgParas->getClass_good3_length()));
    ui->class3WidthSB->setValue(int(pInspectAlgParas->getClass_good3_width()));
}

void QClassSetupDlg::updateInspectParas(QZaoInspectAlgParas *pInspectAlgParas)
{
    if (!pInspectAlgParas)
        return;

    pInspectAlgParas->setClass_good1_length(ui->class1LengthSB->value());
    pInspectAlgParas->setClass_good1_width(ui->class1WidthSB->value());
    pInspectAlgParas->setClass_good2_length(ui->class2LengthSB->value());
    pInspectAlgParas->setClass_good2_width(ui->class2WidthSB->value());
    pInspectAlgParas->setClass_good3_length(ui->class3LengthSB->value());
    pInspectAlgParas->setClass_good3_width(ui->class3WidthSB->value());
}

void QClassSetupDlg::on_buttonBox_accepted()
{
    if (!pInspectAlgParas) {
        accept();
        return;
    }

    updateInspectParas(pInspectAlgParas);

    pInspectAlgParas->save();

    accept();
}
