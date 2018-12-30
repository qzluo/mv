#include "qmodifypassworddlg.h"

QModifyPasswordDlg::QModifyPasswordDlg(QWidget *parent) : QDialog(parent)
{
    adminPw = "";

    QFormLayout* pwLayout = new QFormLayout();

    oldPwLE = new QLineEdit();
    oldPwLE1 = new QLineEdit();
    oldPwLE2 = new QLineEdit();
    oldPwLE->setEchoMode(QLineEdit::Password);
    oldPwLE1->setEchoMode(QLineEdit::Password);
    oldPwLE2->setEchoMode(QLineEdit::Password);

    pwLayout->addRow(new QLabel(tr("Input new Password")), oldPwLE);
    pwLayout->addRow(new QLabel(tr("Input new Password")), oldPwLE1);
    pwLayout->addRow(new QLabel(tr("Comfirm new Password")), oldPwLE2);

    //btn
    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QModifyPasswordDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QModifyPasswordDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->setSpacing(20);
    topLayout->addLayout(pwLayout);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Modify Password"));
}

void QModifyPasswordDlg::onOkBtnClicked()
{
    if (oldPwLE->text().compare(adminPw)) {
        QMessageBox::information(NULL, tr("Modify Password"),
                                 QString(tr("The input new password is incorrect!")));
        return;
    }

    if (oldPwLE1->text().compare(oldPwLE2->text())) {
        QMessageBox::information(NULL, tr("Modify Password"),
                                 QString(tr("Password and Confirm Password inconsistent!")));
        return;
    }

    adminPw = oldPwLE1->text();

    accept();
}

QString QModifyPasswordDlg::getAdminPw() const
{
    return adminPw;
}

void QModifyPasswordDlg::setAdminPw(const QString &value)
{
    adminPw = value;
}
