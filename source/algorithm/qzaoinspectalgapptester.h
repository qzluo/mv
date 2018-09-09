#ifndef QZAOINSPECTALGAPPTESTER_H
#define QZAOINSPECTALGAPPTESTER_H

#include <QObject>

#include "qzaoinspectalgapp.h"


class QZaoInspectAlgAppTester : public QObject
{
    Q_OBJECT
public:
    explicit QZaoInspectAlgAppTester(QObject *parent = 0);

    void testInit();
    int testCalctZaoClass(void);
    int testCalcZaoRegionId(void);
    int testZaoInspect(void);
    int testInspect(void);
    int testInspectSigleImage(void);

signals:

public slots:

private:
    QZaoInspectAlgApp inst;

};

#endif // QZAOINSPECTALGAPPTESTER_H
