#ifndef QREALTIMERESULTWIDGET_H
#define QREALTIMERESULTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class QResultItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QResultItemWidget(QString name, QWidget *parent = 0);

};

class QRealTimeResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRealTimeResultWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // QREALTIMERESULTWIDGET_H
