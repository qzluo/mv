#ifndef QINSPECTCTL_H
#define QINSPECTCTL_H

#include <QObject>
#include <QImage>
#include <QThread>

#include "algorithmmanager.h"


class QInspectWorker : public QObject
{
    Q_OBJECT

public:
    explicit QInspectWorker(QObject *parent = 0);

    AlgorithmManager *getAlgorithmManager() const;
    void setAlgorithmManager(AlgorithmManager *value);

public slots:
    void onHasImage(QImage image);
    void onImageCacheFull();

signals:
    void inspectDone();
//    void InspectMessage(EMessageType msgType);

private:
    AlgorithmManager* pAlgorithmManager;
};

class QInspectCtl : public QObject
{
    Q_OBJECT
public:
    explicit QInspectCtl(QObject *parent = 0);
    ~QInspectCtl();

    int initThread();
    void setAlgorithmManager(AlgorithmManager *value);

signals:
    void hasImage(QImage image);
    void inspectDone();

public slots:

private:
    QInspectWorker* inspectWorker;
    QThread inspectThread;
};

#endif // QINSPECTCTL_H
