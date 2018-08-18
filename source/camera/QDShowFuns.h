#ifndef QDSHOWFUNS_H
#define QDSHOWFUNS_H

#include <atlbase.h>
#include "dshow.h"

#include <QStringList>
#include <QVector>

class QDShowFuns
{
public:
    QDShowFuns();

    // return Null -- 没找到对应 nCamID 的 Moniker 时
    static IMoniker * getCameraMoniker(int nCamID);
    static IBaseFilter* getSoureFilter(int nCamID);
    static int getCameraCount();
    static int getCameraName(int nCamID, char* sName, int nBufferSize);

    static int getDeviceResolusions(int nCamId, QVector<int>& widths, QVector<int>& heights);
    static int getDeviceMaxResolusion(int nCamId, int& width, int& height);

    static void showPropertyPage(int nCamID);

    //亮度
    static int getBrightness(IBaseFilter* capFilter, int* pBrightness);
    static int setBrightness(IBaseFilter* capFilter, int iBrightness);

    static int getBrightnessRange(IBaseFilter* capFilter, int* pMin, int* pMax);

    static int getBrightness(int nCamID, int* pBrightness);
    static int setBrightness(int nCamID, int iBrightness);

    static int getBrightnessRange(int nCamID, int* pMin, int* pMax);
};

#endif // QDSHOWFUNS_H
