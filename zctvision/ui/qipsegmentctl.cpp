#include "qipsegmentctl.h"

#include <QRegExpValidator>
#include <QPainter>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>

QIPSegmentCtl::QIPSegmentCtl(QWidget *parent)
    : QFrame(parent)
{
    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Sunken );

    QRegExp rx("(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})");
    QHBoxLayout *pHBox = new QHBoxLayout(this);
    pHBox->setSpacing(0);
    pHBox->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 3; i++) {
        m_lineEidt[i] = new QLineEdit(this);
        m_lineEidt[i]->setFrame(false);
        m_lineEidt[i]->setMaxLength(3);
        m_lineEidt[i]->setAlignment(Qt::AlignCenter);
        m_lineEidt[i]->installEventFilter(this);
        m_lineEidt[i]->setValidator(new QRegExpValidator(rx, this));
        m_lineEidt[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pHBox->addWidget(m_lineEidt[i]);

        if ( i != 2 ) {
            QLabel* pDot = new QLabel( ".", this);
            pDot->setStyleSheet("background: white");
            pHBox->addWidget(pDot);
            pHBox->setStretch(pHBox->count(), 0);
        }
    }

    setMaximumWidth(120);
}

QIPSegmentCtl::~QIPSegmentCtl()
{

}

int QIPSegmentCtl::getIndex(QLineEdit *pEdit)
{
    int index = -1;
    for (int i = 0; i < 3; i++)
    {
        if (pEdit == m_lineEidt[i])
            index = i;
    }
    return index;
}

bool QIPSegmentCtl::eventFilter(QObject *obj, QEvent *ev)
{
    if (children().contains(obj) && QEvent::KeyPress == ev->type())
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);
        QLineEdit *pEdit = qobject_cast<QLineEdit *>(obj);
        switch (keyEvent->key())
        {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        {
            QString strText = pEdit->text();
            if (pEdit->selectedText().length())
            {
                pEdit->text().replace(pEdit->selectedText(), QChar(keyEvent->key()));
            }
            else if (strText.length() == 3 || strText.length() < 3 && strText.toInt() * 10 > 255)
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 3)
                {
                    m_lineEidt[index + 1]->setFocus();
                    m_lineEidt[index + 1]->selectAll();
                }
            }
            else if (strText.length() == 2 && strText.toInt() * 10 < 255)
            {
                if (Qt::Key_0 == keyEvent->key() && strText.toInt())
                {
                    pEdit->setText(strText.insert(pEdit->cursorPosition(), QChar(Qt::Key_0)));
                }
            }
            return __super::eventFilter(obj, ev);
        }
        break;
        case Qt::Key_Backspace:
        {
            QString strText = pEdit->text();
            if (!strText.length() || strText.length() && !pEdit->cursorPosition())
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 0)
                {
                    m_lineEidt[index - 1]->setFocus();
                    int length = m_lineEidt[index - 1]->text().length();
                    m_lineEidt[index - 1]->setCursorPosition(length ? length : 0);
                }
            }
            return __super::eventFilter(obj, ev);
        }
        case Qt::Key_Left:
        {
            if (!pEdit->cursorPosition())
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 0)
                {
                    m_lineEidt[index - 1]->setFocus();
                    int length = m_lineEidt[index - 1]->text().length();
                    m_lineEidt[index - 1]->setCursorPosition(length ? length : 0);
                }
            }
            return __super::eventFilter(obj, ev);
        }
        case Qt::Key_Right:
        {
            if (pEdit->cursorPosition() == pEdit->text().length())
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 3)
                {
                    m_lineEidt[index + 1]->setFocus();
                    m_lineEidt[index + 1]->setCursorPosition(0);
                }
            }
            return __super::eventFilter(obj, ev);
        }
        default:
            break;
        }
    }
    return false;
}

void QIPSegmentCtl::setText(const QString &strIP)
{
    if (!isTextValid(strIP))
    {
        QMessageBox::warning(this, "Attention", "Your IP Address is Invalid!", QMessageBox::StandardButton::Ok);
        return;
    }
    else
    {
        int i = 0;
        QStringList ipList = strIP.split(".");
        for (i = 0; i < ipList.size(); ++i) {
            m_lineEidt[i]->setText(ipList[i]);
        }
    }
}

bool QIPSegmentCtl::isTextValid(const QString &strIP)
{
    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){2}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if (!rx2.exactMatch(strIP))
        return false;
    return true;
}

QString QIPSegmentCtl::text() const
{
    QString strIP;
    for (int i = 0; i < 2; i++) {
        strIP.append(m_lineEidt[i]->text());
        strIP.append(".");
    }
    strIP.append(m_lineEidt[2]->text());

    return strIP;
}
