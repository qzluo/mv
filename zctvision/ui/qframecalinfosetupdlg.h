#ifndef QFRAMECALINFOSETUPDLG_H
#define QFRAMECALINFOSETUPDLG_H

#include <QDialog>
#include <QtWidgets>

#include "qframecalinfo.h"

class QFrameCalInfoSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QFrameCalInfoSetupDlg(QWidget *parent = 0);

    QFrameCalInfo *getPFramCalInfo() const;
    void setPFramCalInfo(QFrameCalInfo *value);

signals:

public slots:
    void onOkBtnClicked();

private:
    QButtonGroup* dirBtnGroup;
    QDoubleSpinBox* pixelPerMmDSB;
    QDoubleSpinBox* distBtwFrmsDSB;

    QFrameCalInfo* pFramCalInfo;
};

#endif // QFRAMECALINFOSETUPDLG_H
