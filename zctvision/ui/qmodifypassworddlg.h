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
    QLineEdit* newPwLE1;
    QLineEdit* newPwLE2;
};

#endif // QMODIFYPASSWORDDLG_H
