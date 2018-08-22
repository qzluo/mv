#ifndef QSYSDEFINE_H
#define QSYSDEFINE_H

#include <QString>

#define ZAO_CLASS_GOOD1        0   //特等枣
#define ZAO_CLASS_GOOD2        1   //一等枣
#define ZAO_CLASS_GOOD3        2   //二等枣
#define ZAO_CLASS_GOOD4        3   //三等枣

#define ZAO_CLASS_BAD1         4   //皮皮枣
#define ZAO_CLASS_BAD2         5   //裂口枣
#define ZAO_CLASS_BAD3         6   //鸟啄枣
#define ZAO_CLASS_BAD4         7   //烂枣

#define ZAO_CLASS_NONE         15  //无枣

class QSysDefine
{
public:
    QSysDefine();

    static QString GetSettingSysFileName();
    static QString GetFrameCalInfoFileName();
    static QString GetInspectAlgParasFileName();
    static QString GetRecognizeModelFilePath();
    static QString GetRecognizeModelFileName();
};

#endif // QSYSDEFINE_H
