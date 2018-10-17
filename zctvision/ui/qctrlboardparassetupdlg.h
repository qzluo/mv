#ifndef QCTRLBOARDPARASSETUPDLG_H
#define QCTRLBOARDPARASSETUPDLG_H

#include <QtWidgets>

class QCtrlBoardParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QCtrlBoardParasSetupDlg(QWidget *parent = 0);

signals:

public slots:

private:
    QLineEdit* registerConts[8];
};

#endif // QCTRLBOARDPARASSETUPDLG_H
