#ifndef QREALTIMERESULTWIDGET_H
#define QREALTIMERESULTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class QResultItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QResultItemWidget(QString name, QWidget *parent = 0);

    void setResult(QString& result, QString& length, QString& diameter);

private:
    QLineEdit* resultLE;
    QLineEdit* lengthLE;
    QLineEdit* diameterLE;
};

class QRealTimeResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRealTimeResultWidget(QWidget *parent = 0);

    void setItemResult(int itemIndex, QString& result, QString& length, QString& diameter);

signals:

public slots:

private:
    QResultItemWidget* resultItems[8];
};

#endif // QREALTIMERESULTWIDGET_H
