#include "qmodifypassworddlg.h"

QModifyPasswordDlg::QModifyPasswordDlg(QWidget *parent) : QDialog(parent)
{
    adminPw = "";

    QFormLayout* pwLayout = new QFormLayout();

    oldPwLE = new QLineEdit();
    newPwLE1 = new QLineEdit();
    newPwLE2 = new QLineEdit();
    oldPwLE->setEchoMode(QLineEdit::Password);
    newPwLE1->setEchoMode(QLineEdit::Password);
    newPwLE2->setEchoMode(QLineEdit::Password);

    pwLayout->addRow(new QLabel(tr("Input Old Password")), oldPwLE);
    pwLayout->addRow(new QLabel(tr("Input New Password")), newPwLE1);
    pwLayout->addRow(new QLabel(tr("Comfirm New Password")), newPwLE2);

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
                                 QString(tr("The input old password is incorrect!")));
        return;
    }

    if (newPwLE1->text().compare(newPwLE2->text())) {
        QMessageBox::information(NULL, tr("Modify Password"),
                                 QString(tr("Password and Confirm Password inconsistent!")));
        return;
    }

    adminPw = newPwLE1->text();

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
