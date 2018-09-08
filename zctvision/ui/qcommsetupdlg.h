#ifndef QCOMMSETUPDLG_H
#define QCOMMSETUPDLG_H

#include <QDialog>
#include <QtWidgets>

#include "projectsysinfo.h"

class QCommSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QCommSetupDlg(QWidget *parent = 0);

    ProjectSysInfo *getPSysInfo() const;
    void setPSysInfo(ProjectSysInfo *value);

signals:

public slots:
    void onOkBtnClicked();

private:
    QComboBox* portCB;
    QComboBox* baudCB;

    ProjectSysInfo* pSysInfo;
};

#endif // QCOMMSETUPDLG_H
