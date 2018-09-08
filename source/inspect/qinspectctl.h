#ifndef QINSPECTCTL_H
#define QINSPECTCTL_H

#include <QObject>
#include <QImage>
#include <QThread>

#include "qzaoinspectalgapp.h"


class QInspectWorker : public QObject
{
    Q_OBJECT

public:
    explicit QInspectWorker(QObject *parent = 0);

    QZaoInspectAlgApp *getAlgorithmManager() const;
    void setAlgorithmManager(QZaoInspectAlgApp *value);

public slots:
    void onHasImage(QImage image);
    void onImageCacheFull();

signals:
    void inspectDone();
//    void InspectMessage(EMessageType msgType);

private:
    QZaoInspectAlgApp* pAlgorithmManager;
};

class QInspectCtl : public QObject
{
    Q_OBJECT
public:
    explicit QInspectCtl(QObject *parent = 0);
    ~QInspectCtl();

    int initThread();
    void setAlgorithmManager(QZaoInspectAlgApp *value);

signals:
    void hasImage(QImage image);
    void inspectDone();

public slots:

private:
    QInspectWorker* inspectWorker;
    QThread inspectThread;
};

#endif // QINSPECTCTL_H
