#include "qselectcameratypedlg.h"

QSelectCameraTypeDlg::QSelectCameraTypeDlg(QWidget *parent) : QDialog(parent)
{
    pSysInfo = NULL;

    QGroupBox* cameraTypeGroup = new QGroupBox(tr("Select Camera Type"), this);
    cameraTypeCB = new QComboBox(this);
    cameraTypeCB->addItem(tr("None"));
    cameraTypeCB->addItem(tr("Avt"));
    cameraTypeCB->addItem(tr("Daheng"));

    cameraTypeCB->setCurrentIndex(0);

    QHBoxLayout* groupLayout = new QHBoxLayout;
    groupLayout->addWidget(new QLabel(tr("Camera Type: "), this));
    groupLayout->addWidget(cameraTypeCB);
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
    int cameraType = pSysInfo->getCameraType();
    if (cameraType < 3)
        cameraTypeCB->setCurrentIndex(cameraType);
}

void QSelectCameraTypeDlg::onOkBtnClicked()
{
    int cameraType = cameraTypeCB->currentIndex();
    if (pSysInfo->getCameraType() != cameraType) {
        pSysInfo->setCameraType(cameraType);
        pSysInfo->save();
    }

    accept();
}
