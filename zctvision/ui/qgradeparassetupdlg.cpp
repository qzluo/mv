#include "qgradeparassetupdlg.h"

QGradeParasSetupDlg::QGradeParasSetupDlg(QWidget *parent) : QDialog(parent)
{
    pInspectAlgParas = NULL;

    //Good Jujube Parameters
    //1
    QLabel* class1LengthLbl = new QLabel(tr("Class1 Length: "));
    class1LengthDSB = new QDoubleSpinBox(this);
    class1LengthDSB->setRange(0, 10000);
    QLabel* class1LengthUnit = new QLabel("mm");

    QLabel* class1WidthLbl = new QLabel(tr("Class1 Width: "));
    class1WidthDSB = new QDoubleSpinBox(this);
    class1WidthDSB->setRange(0, 10000);
    QLabel* class1WidthUnit = new QLabel("mm");

    QHBoxLayout* class1Layout = new QHBoxLayout();
    class1Layout->addWidget(class1LengthLbl);
    class1Layout->addWidget(class1LengthDSB);
    class1Layout->addWidget(class1LengthUnit);
    class1Layout->addSpacing(10);
    class1Layout->addWidget(class1WidthLbl);
    class1Layout->addWidget(class1WidthDSB);
    class1Layout->addWidget(class1WidthUnit);

    //2
    QLabel* class2LengthLbl = new QLabel(tr("Class2 Length: "));
    class2LengthDSB = new QDoubleSpinBox(this);
    class2LengthDSB->setRange(0, 10000);
    QLabel* class2LengthUnit = new QLabel("mm");

    QLabel* class2WidthLbl = new QLabel(tr("Class2 Width: "));
    class2WidthDSB = new QDoubleSpinBox(this);
    class2WidthDSB->setRange(0, 10000);
    QLabel* class2WidthUnit = new QLabel("mm");

    QHBoxLayout* class2Layout = new QHBoxLayout();
    class2Layout->addWidget(class2LengthLbl);
    class2Layout->addWidget(class2LengthDSB);
    class2Layout->addWidget(class2LengthUnit);
    class2Layout->addSpacing(10);
    class2Layout->addWidget(class2WidthLbl);
    class2Layout->addWidget(class2WidthDSB);
    class2Layout->addWidget(class2WidthUnit);

    //3
    QLabel* class3LengthLbl = new QLabel(tr("Class3 Length: "));
    class3LengthDSB = new QDoubleSpinBox(this);
    class3LengthDSB->setRange(0, 10000);
    QLabel* class3LengthUnit = new QLabel("mm");

    QLabel* class3WidthLbl = new QLabel(tr("Class3 Width: "));
    class3WidthDSB = new QDoubleSpinBox(this);
    class3WidthDSB->setRange(0, 10000);
    QLabel* class3WidthUnit = new QLabel("mm");

    QHBoxLayout* class3Layout = new QHBoxLayout();
    class3Layout->addWidget(class3LengthLbl);
    class3Layout->addWidget(class3LengthDSB);
    class3Layout->addWidget(class3LengthUnit);
    class3Layout->addSpacing(10);
    class3Layout->addWidget(class3WidthLbl);
    class3Layout->addWidget(class3WidthDSB);
    class3Layout->addWidget(class3WidthUnit);

    QGroupBox* goodJujubeParasGB = new QGroupBox(tr("Good Jujube Parameters"), this);
    QVBoxLayout* goodJujubeParasLayout = new QVBoxLayout;
    goodJujubeParasLayout->addLayout(class1Layout);
    goodJujubeParasLayout->addLayout(class2Layout);
    goodJujubeParasLayout->addLayout(class3Layout);
    goodJujubeParasGB->setLayout(goodJujubeParasLayout);

    //btn
    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QGradeParasSetupDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QGradeParasSetupDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->addWidget(goodJujubeParasGB);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Grade Parameters Setup"));
}

QZaoInspectAlgParas *QGradeParasSetupDlg::getPInspectAlgParas() const
{
    return pInspectAlgParas;
}

void QGradeParasSetupDlg::setPInspectAlgParas(QZaoInspectAlgParas *value)
{
    if (!value)
        return;

    pInspectAlgParas = value;

    class1LengthDSB->setValue(pInspectAlgParas->getClass_good1_length());
    class1WidthDSB->setValue(pInspectAlgParas->getClass_good1_width());
    class2LengthDSB->setValue(pInspectAlgParas->getClass_good2_length());
    class2WidthDSB->setValue(pInspectAlgParas->getClass_good2_width());
    class3LengthDSB->setValue(pInspectAlgParas->getClass_good3_length());
    class3WidthDSB->setValue(pInspectAlgParas->getClass_good3_width());
}

void QGradeParasSetupDlg::updateInspectParas(QZaoInspectAlgParas *pInspectAlgParas)
{
    if (!pInspectAlgParas)
        return;

    pInspectAlgParas->setClass_good1_length(class1LengthDSB->value());
    pInspectAlgParas->setClass_good1_width(class1WidthDSB->value());
    pInspectAlgParas->setClass_good2_length(class2LengthDSB->value());
    pInspectAlgParas->setClass_good2_width(class2WidthDSB->value());
    pInspectAlgParas->setClass_good3_length(class3LengthDSB->value());
    pInspectAlgParas->setClass_good3_width(class3WidthDSB->value());
}

void QGradeParasSetupDlg::onOkBtnClicked()
{
    if (!pInspectAlgParas) {
        accept();
        return;
    }

    updateInspectParas(pInspectAlgParas);

    pInspectAlgParas->save();

    accept();

}
