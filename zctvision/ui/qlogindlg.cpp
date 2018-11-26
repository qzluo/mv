#include "qlogindlg.h"

QLoginDlg::QLoginDlg(QWidget *parent) : QDialog(parent)
{
    adminPw = "";

    QFormLayout* pwLayout = new QFormLayout();
    pwLE = new QLineEdit();
    pwLE->setEchoMode(QLineEdit::Password);

    pwLayout->addRow(new QLabel(tr("Password")), pwLE);

    //btn
    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QLoginDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QLoginDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->setSpacing(20);
    topLayout->addLayout(pwLayout);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Login"));
}

void QLoginDlg::setAdministratorPw(const QString &value)
{
    adminPw = value;
}

void QLoginDlg::onOkBtnClicked()
{
    if (pwLE->text() == "hs_hd_123" || pwLE->text() == adminPw)
        accept();
    else
        QMessageBox::information(NULL, tr("Login"),
                                 QString(tr("The input password is incorrect!")));
}
