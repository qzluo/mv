#ifndef QLOGINDLG_H
#define QLOGINDLG_H

#include <QDialog>
#include <QtWidgets>

class QLoginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QLoginDlg(QWidget *parent = 0);
    void setAdministratorPw(const QString &value);

signals:

public slots:
    void onOkBtnClicked();

private:
    QLineEdit* pwLE;
    QString adminPw;

};

#endif // QLOGINDLG_H
