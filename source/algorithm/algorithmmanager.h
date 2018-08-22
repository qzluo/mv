#ifndef ALGORITHMMANAGER_H
#define ALGORITHMMANAGER_H

#include "qzaoinspectalgapp.h"

#include <QObject>
#include <QImage>

class AlgorithmManager : public QObject
{
    Q_OBJECT
public:
    explicit AlgorithmManager(QObject *parent = 0);

    int init(void);
    int reset(void);

    int inspect(const QImage &cameraImg, QImage& outImg = QImage());

    int getDataVariant(const QString& dataName, QVariant& var);

    QFrameCalInfo* getPFrameCalInfo(void);
    QZaoInspectAlgParas* getPInspectAlgParas(void);

signals:

public slots:

private:
    QZaoInspectAlgApp zaoInspectAlg;
};

#endif // ALGORITHMMANAGER_H
