#include "qframecalinfosetupdlg.h"

QFrameCalInfoSetupDlg::QFrameCalInfoSetupDlg(QWidget *parent) : QDialog(parent)
{
    pFramCalInfo = NULL;

    QGroupBox* frameDirGB = new QGroupBox(tr("Image Flow Direction"), this);
    QRadioButton* downBtn = new QRadioButton(tr("Down"), this);
    QRadioButton* rightBtn = new QRadioButton(tr("Right"), this);
    QRadioButton* upBtn = new QRadioButton(tr("up"), this);
    QRadioButton* leftBtn = new QRadioButton(tr("left"), this);
    dirBtnGroup = new QButtonGroup(this);
    dirBtnGroup->addButton(downBtn, 1);
    dirBtnGroup->addButton(rightBtn, 2);
    dirBtnGroup->addButton(upBtn, 3);
    dirBtnGroup->addButton(leftBtn, 4);
    dirBtnGroup->button(1)->setChecked(true);

    QVBoxLayout* frameDirLayout = new QVBoxLayout;
    frameDirLayout->addWidget(downBtn);
    frameDirLayout->addWidget(rightBtn);
    frameDirLayout->addWidget(upBtn);
    frameDirLayout->addWidget(leftBtn);

    frameDirGB->setLayout(frameDirLayout);

//    QLabel* pixelPerMm = new QLabel(tr("Pixel per mm: "), this);
//    pixelPerMmDSB = new QDoubleSpinBox(this);
//    pixelPerMmDSB->setRange(0, 10000);
//    QHBoxLayout* pixelUnitLayout = new QHBoxLayout();
//    pixelUnitLayout->addWidget(pixelPerMm);
//    pixelUnitLayout->addWidget(pixelPerMmDSB);

//    QLabel* distBtwFrms = new QLabel(tr("Distance Between Frames: "), this);
//    distBtwFrmsDSB = new QDoubleSpinBox(this);
//    distBtwFrmsDSB->setRange(0, 10000);
//    QLabel* unitLabel = new QLabel(tr("mm"), this);
//    QHBoxLayout* distBtwFrmsLayout = new QHBoxLayout();
//    distBtwFrmsLayout->addWidget(distBtwFrms);
//    distBtwFrmsLayout->addWidget(distBtwFrmsDSB);
//    distBtwFrmsLayout->addWidget(unitLabel);

    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QFrameCalInfoSetupDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QFrameCalInfoSetupDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);


    QVBoxLayout* topLayout = new QVBoxLayout;
    topLayout->addWidget(frameDirGB);
//    topLayout->addLayout(pixelUnitLayout);
//    topLayout->addLayout(distBtwFrmsLayout);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Image Calibration Setup"));
}

QFrameCalInfo *QFrameCalInfoSetupDlg::getPFramCalInfo() const
{
    return pFramCalInfo;
}

void QFrameCalInfoSetupDlg::setPFramCalInfo(QFrameCalInfo *value)
{
    if (!value)
        return;

    pFramCalInfo = value;

    dirBtnGroup->button(pFramCalInfo->getFrameDir() + 1)->setChecked(true);
//    pixelPerMmDSB->setValue(pFramCalInfo->getFactorMmToPix());
//    distBtwFrmsDSB->setValue(pFramCalInfo->getDistBtFrms());
}

void QFrameCalInfoSetupDlg::onOkBtnClicked()
{
    if (!pFramCalInfo) {
        accept();
        return;
    }

    int frameDir = dirBtnGroup->checkedId() - 1;
    pFramCalInfo->setFrameDir(frameDir);
//    pFramCalInfo->setFactorMmToPix(pixelPerMmDSB->value());
//    pFramCalInfo->setDistBtFrms(distBtwFrmsDSB->value());

    pFramCalInfo->save();

    accept();
}
