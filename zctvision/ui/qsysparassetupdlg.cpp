#include "qsysparassetupdlg.h"
#include "qmodifypassworddlg.h"

QSysParasSetupDlg::QSysParasSetupDlg(QWidget *parent) : QDialog(parent)
{
    adminPw = "";
    pSysInfo = NULL;

    outputIsOpenedCB = new QCheckBox(tr("Output In Running"), this);
    saveImageOpenedCB = new QCheckBox(tr("Save Image In Running"), this);
    logOutputIsOpenedCB = new QCheckBox(tr("Record Output In Running"), this);

    tmpFilePathLE = new QLineEdit(this);
    QPushButton* selectPathBtn = new QPushButton(tr("Select ..."), this);
    QHBoxLayout* tmpFilePathLayout = new QHBoxLayout();
    tmpFilePathLayout->addWidget(new QLabel(tr("Save Image File Path:")));
    tmpFilePathLayout->addWidget(tmpFilePathLE);
    tmpFilePathLayout->addWidget(selectPathBtn);

    connect(selectPathBtn, &QPushButton::clicked,
            this, &QSysParasSetupDlg::onSelectPathBtnClicked);

    tmpNetworkSegmentLE = new QIPSegmentCtl(this);
    QHBoxLayout* tmpNetWorkSegmentLayout = new QHBoxLayout();
    tmpNetWorkSegmentLayout->addWidget(new QLabel(tr("Network Segment:")));
    tmpNetWorkSegmentLayout->addWidget(tmpNetworkSegmentLE);
    tmpNetWorkSegmentLayout->addStretch();

    QPushButton* modifyAdminPwBtn = new QPushButton(tr("Modify Administrator Password"),
                                                    this);
    connect(modifyAdminPwBtn, &QPushButton::clicked,
            this, &QSysParasSetupDlg::onModifyAdminPwBtnClicked);

    //ok btn
    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);

    connect(okBtn, &QPushButton::clicked,
            this, &QSysParasSetupDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QSysParasSetupDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout;

    topLayout->addWidget(outputIsOpenedCB);
    topLayout->addWidget(saveImageOpenedCB);
    topLayout->addWidget(logOutputIsOpenedCB);
    topLayout->addLayout(tmpFilePathLayout);
    topLayout->addLayout(tmpNetWorkSegmentLayout);
    topLayout->addWidget(modifyAdminPwBtn);
    topLayout->addSpacing(10);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("System Parameters Setup"));
}

void QSysParasSetupDlg::onOkBtnClicked()
{
    bool outputIsOpened = pSysInfo->getOutputIsOpened();
    bool saveImageOpened = pSysInfo->getSaveImageIsOpened();
    bool logOutputIsOpened = pSysInfo->getLogOutputIsOpened();
    QString tmeFilePath = pSysInfo->getTmpFilePath();
    QString tmeNetworkSegment = pSysInfo->getNetworkSegment();
    QString tmpAdminPw = pSysInfo->getAdministratorPw();
    bool changed = false;

    if (outputIsOpenedCB->isChecked() != outputIsOpened) {
        pSysInfo->setOutputIsOpened(!outputIsOpened);
        changed = true;
    }

    if (saveImageOpenedCB->isChecked() != saveImageOpened) {
        pSysInfo->setSaveImageIsOpened(!saveImageOpened);
        changed = true;
    }

    if (logOutputIsOpenedCB->isChecked() != logOutputIsOpened) {
        pSysInfo->setLogOutputIsOpened(!logOutputIsOpened);
        changed = true;
    }

    if (tmpFilePathLE->text().compare(tmeFilePath)) {
        pSysInfo->setTmpFilePath(tmpFilePathLE->text());
        changed = true;
    }

    if (tmpNetworkSegmentLE->text().compare(tmeNetworkSegment)) {
        pSysInfo->setNetworkSegment(tmpNetworkSegmentLE->text());
        changed = true;
    }

    if (adminPw.compare(tmpAdminPw)) {
        pSysInfo->setAdministratorPw(adminPw);
        changed = true;
    }

    if (changed)
        pSysInfo->save();

    accept();
}

void QSysParasSetupDlg::onSelectPathBtnClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    tmpFilePathLE->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty())
        return;

    tmpFilePathLE->setText(dir);
}

void QSysParasSetupDlg::onModifyAdminPwBtnClicked()
{
    QModifyPasswordDlg dlg;
    dlg.setAdminPw(adminPw);
    if (dlg.exec() == QDialog::Accepted) {
        adminPw = dlg.getAdminPw();
    }
}

ProjectSysInfo *QSysParasSetupDlg::getPSysInfo() const
{
    return pSysInfo;
}

void QSysParasSetupDlg::setPSysInfo(ProjectSysInfo *value)
{
    pSysInfo = value;

    if (!pSysInfo)
        return;

    outputIsOpenedCB->setChecked(pSysInfo->getOutputIsOpened());
    saveImageOpenedCB->setChecked(pSysInfo->getSaveImageIsOpened());
    logOutputIsOpenedCB->setChecked(pSysInfo->getLogOutputIsOpened());
    tmpFilePathLE->setText(pSysInfo->getTmpFilePath());
    tmpNetworkSegmentLE->setText(pSysInfo->getNetworkSegment());
    adminPw = pSysInfo->getAdministratorPw();
}
