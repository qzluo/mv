#ifndef QIPSEGMENTCTL_H
#define QIPSEGMENTCTL_H


#include <QtWidgets>
#include <QEvent>

class QIPSegmentCtl : public QFrame
{
    Q_OBJECT

public:
    QIPSegmentCtl(QWidget *parent = 0);
    ~QIPSegmentCtl();

    void setText(const QString &segment);
    QString text() const;

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

    int getIndex(QLineEdit *pEdit);
    bool isTextValid(const QString &strIP);

private:
    QLineEdit *m_lineEidt[3];
};


#endif // QIPSEGMENTCTL_H
