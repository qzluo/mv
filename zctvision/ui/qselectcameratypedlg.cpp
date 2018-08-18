#include "qselectcameratypedlg.h"

QSelectCameraTypeDlg::QSelectCameraTypeDlg(QWidget *parent) : QDialog(parent)
{
    pSysInfo = NULL;

    QGroupBox* cameraTypeGroup = new QGroupBox(tr("Select Camera Type"), this);
    cameraType_avt = new QRadioButton(tr("avt"), this);
    cameraType_daheng = new QRadioButton(tr("daheng"), this);

    QVBoxLayout* groupLayout = new QVBoxLayout;
    groupLayout->addWidget(cameraType_avt);
    groupLayout->addWidget(cameraType_daheng);
    cameraTypeGroup->setLayout(groupLayout);

    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);

    connect(okBtn, &QPushButton::clicked,
            this, &QSelectCameraTypeDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QSelectCameraTypeDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout;

    topLayout->addWidget(cameraTypeGroup);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Select Camera Type"));
}

ProjectSysInfo *QSelectCameraTypeDlg::getPSysInfo() const
{
    return pSysInfo;
}

void QSelectCameraTypeDlg::setPSysInfo(ProjectSysInfo *value)
{
    pSysInfo = value;
    if (pSysInfo->getCameraType() == 0)
        cameraType_avt->setChecked(true);
    else
        cameraType_daheng->setChecked(true);
}

void QSelectCameraTypeDlg::onOkBtnClicked()
{
    int cameraType = cameraType_avt->isChecked() ? 0 : 1;
    if (pSysInfo->getCameraType() != cameraType) {
        pSysInfo->setCameraType(cameraType);
        pSysInfo->save();
    }

    accept();
}
