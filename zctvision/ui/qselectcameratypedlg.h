#ifndef QSELECTCAMERATYPE_H
#define QSELECTCAMERATYPE_H

#include <QDialog>
#include <QtWidgets>

#include "projectsysinfo.h"

class QSelectCameraTypeDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QSelectCameraTypeDlg(QWidget *parent = 0);

    ProjectSysInfo *getPSysInfo() const;
    void setPSysInfo(ProjectSysInfo *value);

signals:

public slots:
    void onOkBtnClicked();

private:
    QRadioButton* cameraType_avt;
    QRadioButton* cameraType_daheng;

    ProjectSysInfo* pSysInfo;
};

#endif // QSELECTCAMERATYPE_H
