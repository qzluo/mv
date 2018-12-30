#ifndef QMODIFYPASSWORDDLG_H
#define QMODIFYPASSWORDDLG_H

#include <QDialog>
#include <QtWidgets>

class QModifyPasswordDlg : public QDialog
{
    Q_OBJECT
public:
    explicit QModifyPasswordDlg(QWidget *parent = 0);

    QString getAdminPw() const;
    void setAdminPw(const QString &value);

signals:

public slots:
    void onOkBtnClicked();

private:
    QString adminPw;

    QLineEdit* oldPwLE;
    QLineEdit* oldPwLE1;
    QLineEdit* oldPwLE2;
};

#endif // QMODIFYPASSWORDDLG_H
