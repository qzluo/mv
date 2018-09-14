#ifndef QSYSPARASSETUPDLG_H
#define QSYSPARASSETUPDLG_H

#include <QtWidgets>
#include "projectsysinfo.h"

class QSysParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QSysParasSetupDlg(QWidget *parent = 0);

    ProjectSysInfo *getPSysInfo() const;
    void setPSysInfo(ProjectSysInfo *value);

signals:

public slots:
    void onOkBtnClicked();
    void onSelectPathBtnClicked();

private:
    QCheckBox* outputIsOpenedCB;
    QCheckBox* saveImageOpenedCB;
    QCheckBox* logOutputIsOpenedCB;
    QLineEdit* tmpFilePathLE;

    ProjectSysInfo* pSysInfo;
};

#endif // QSYSPARASSETUPDLG_H
