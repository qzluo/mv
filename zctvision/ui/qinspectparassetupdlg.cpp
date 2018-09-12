#include "qinspectparassetupdlg.h"

QInspectParasSetupDlg::QInspectParasSetupDlg(QWidget *parent) : QDialog(parent)
{
    pInspectAlgParas = NULL;

    //detect parameter
    //I
    QLabel* iMinLbl = new QLabel(tr("Imin: "));
    iMinSB = new QSpinBox(this);
    iMinSB->setRange(0, 255);

    QLabel* iMaxLbl = new QLabel(tr("Imax: "));
    iMaxSB = new QSpinBox(this);
    iMaxSB->setRange(0, 255);

    QHBoxLayout* iLayout = new QHBoxLayout();
    iLayout->addWidget(iMinLbl);
    iLayout->addWidget(iMinSB);
    iLayout->addSpacing(20);
    iLayout->addWidget(iMaxLbl);
    iLayout->addWidget(iMaxSB);
    iLayout->addStretch();

    //B
    QLabel* bMinLbl = new QLabel(tr("Bmin: "));
    bMinSB = new QSpinBox(this);
    bMinSB->setRange(0, 255);

    QLabel* bMaxLbl = new QLabel(tr("Bmax: "));
    bMaxSB = new QSpinBox(this);
    bMaxSB->setRange(0, 255);

    QHBoxLayout* bLayout = new QHBoxLayout();
    bLayout->addWidget(bMinLbl);
    bLayout->addWidget(bMinSB);
    bLayout->addSpacing(20);
    bLayout->addWidget(bMaxLbl);
    bLayout->addWidget(bMaxSB);
    bLayout->addStretch();

    //R
    QLabel* rMinLbl = new QLabel(tr("Rmin: "));
    rMinSB = new QSpinBox(this);
    rMinSB->setRange(0, 255);

    QLabel* rMaxLbl = new QLabel(tr("Rmax: "));
    rMaxSB = new QSpinBox(this);
    rMaxSB->setRange(0, 255);

    QHBoxLayout* rLayout = new QHBoxLayout();
    rLayout->addWidget(rMinLbl);
    rLayout->addWidget(rMinSB);
    rLayout->addSpacing(20);
    rLayout->addWidget(rMaxLbl);
    rLayout->addWidget(rMaxSB);
    rLayout->addStretch();

    //w,h
    QLabel* wMinLbl = new QLabel(tr("Wmin: "));
    wMinSB = new QSpinBox(this);
    wMinSB->setRange(0, 1000);

    QLabel* hMinLbl = new QLabel(tr("Hmin: "));
    hMinSB = new QSpinBox(this);
    hMinSB->setRange(0, 1000);

    QLabel* ratioMaxLbl = new QLabel(tr("Max Ratio: "));
    ratioMaxSB = new QDoubleSpinBox(this);
    ratioMaxSB->setRange(0, 1000);

    QHBoxLayout* whLayout = new QHBoxLayout();
    whLayout->addWidget(wMinLbl);
    whLayout->addWidget(wMinSB);
    whLayout->addSpacing(20);
    whLayout->addWidget(hMinLbl);
    whLayout->addWidget(hMinSB);
    whLayout->addSpacing(20);
    whLayout->addWidget(ratioMaxLbl);
    whLayout->addWidget(ratioMaxSB);
    whLayout->addStretch();

    QGroupBox* detectParasGB = new QGroupBox(tr("Detect Parameters"), this);
    QVBoxLayout* detectParasLayout = new QVBoxLayout;
    detectParasLayout->addLayout(iLayout);
    detectParasLayout->addLayout(bLayout);
    detectParasLayout->addLayout(rLayout);
    detectParasLayout->addLayout(whLayout);
    detectParasGB->setLayout(detectParasLayout);

    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QInspectParasSetupDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QInspectParasSetupDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->addWidget(detectParasGB);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Algrithm Parameters Setup"));

}

QZaoInspectAlgParas *QInspectParasSetupDlg::getPInspectAlgParas() const
{
    return pInspectAlgParas;
}

void QInspectParasSetupDlg::setPInspectAlgParas(QZaoInspectAlgParas *value)
{
    if (!value)
        return;

    pInspectAlgParas = value;

    iMinSB->setValue(pInspectAlgParas->getImin());
    iMaxSB->setValue(pInspectAlgParas->getImax());
    bMinSB->setValue(pInspectAlgParas->getBmin());
    bMaxSB->setValue(pInspectAlgParas->getBmax());
    rMinSB->setValue(pInspectAlgParas->getRmin());
    rMaxSB->setValue(pInspectAlgParas->getRmax());

    wMinSB->setValue(pInspectAlgParas->getWmin());
    hMinSB->setValue(pInspectAlgParas->getHmin());
    ratioMaxSB->setValue(pInspectAlgParas->getRatioMax());
}

void QInspectParasSetupDlg::updateInspectParas(QZaoInspectAlgParas *pInspectAlgParas)
{
    if (!pInspectAlgParas)
        return;

    pInspectAlgParas->setImin(iMinSB->value());
    pInspectAlgParas->setImax(iMaxSB->value());
    pInspectAlgParas->setBmin(bMinSB->value());
    pInspectAlgParas->setBmax(bMaxSB->value());
    pInspectAlgParas->setRmin(rMinSB->value());
    pInspectAlgParas->setRmax(rMaxSB->value());

    pInspectAlgParas->setWmin(wMinSB->value());
    pInspectAlgParas->setHmin(hMinSB->value());
    pInspectAlgParas->setRatioMax(ratioMaxSB->value());
}

void QInspectParasSetupDlg::onOkBtnClicked()
{
    if (!pInspectAlgParas) {
        reject();
        return;
    }

    updateInspectParas(pInspectAlgParas);

//    pInspectAlgParas->save();

    accept();
}
