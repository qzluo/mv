#ifndef QZAOINSPECTALGAPP_H
#define QZAOINSPECTALGAPP_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QImage>
#include <QMap>
#include <QVariant>

#include "cv.h"

#include "qframecalinfo.h"
#include "qzaoinspectalgparas.h"
#include "qsysdefine.h"

typedef struct tagDetectResult {
    int result;
    int frameId;
    int left_col_result;
    int right_col_result;
    QVariant curFrameZaoInfo;
} DetectResult, *PDetectResult;

//枣信息
typedef struct tagZaoInfo {
    int classId;    //等级
    cv::Rect zaoPos;
} ZaoInfo, *PZaoInfo;

Q_DECLARE_METATYPE(ZaoInfo)


///
/// \brief zaoInspect: 枣检测及识别算法接口
/// \param imageIn -- [in]待检测图片
/// \param zaoInfo -- [out]图片中识别的枣信息
/// \param zaoCount -- [out]共识别出的枣个数
/// \return 0 -- 无异常， -1 -- 异常，错误
///
extern int zaoInspect(cv::Mat imageIn, ZaoInfo zaoInfo[], int* zaoCount);


///
/// \brief The QZaoInspectAlgApp class
/// 1. 检测当前帧的枣信息
/// 2. 处理当前帧的枣信息，包括将好枣根据设置的阈值分类及将枣归入实现划分的区域里
/// 3. 那些区域是为了综合多次枣信息，只传出下一帧将要移出视野的枣信息
/// 4. 综合枣信息需要考虑之前处理的帧号，及当前检测的帧号，枣在两个连续帧间移动一个划分区域
class QZaoInspectAlgApp
{
public:
    QZaoInspectAlgApp();

    enum EResDataId {
        E_Inspect_Result = 0,
        E_Cur_FrameId,
        E_Left_Col_Result,
        E_Right_Col_Result,
        E_Cur_Frame_Product_Info,
    };

    //枣所属列
    enum EZAO_COL_POS {
        ZAO_COL_POS_LEFT = 0,
        ZAO_COL_POS_RIGHT,
    };

    int init(void);
    int loadCfgFile(void);

    int inspect(const QImage &cameraImg, QImage& outImg = QImage());

    //获取指定的检测结果
    int getDataVariant(const QString& dataName, QVariant& var);

    //更新检测结果
    void setDataVariant(const QString& dataName, QVariant& var);

    static QString getResDataDescFromId(EResDataId eResDataId);

    QFrameCalInfo* getPFrameCalInfo(void);
    QZaoInspectAlgParas* getPInspectAlgParas(void);
//    QZaoInspectAlgParas

//private:

    void reset(void);

    //计算区域数，将图片分割成一条条带状，物料将依次在带中移动
    int calcRegionCount();

    //计算枣等级，主要是区分出好枣的等级
    int calctZaoClass(ZaoInfo zaoInfo, int* zaoClass);

    //计算枣所属的区域Id
    int calcZaoRegionId(ZaoInfo zaoInfo, int* leftOrRight, int* regionId);

    //综合枣等级
    int mergeZaoClasses(int class1, int class2, int* retClass);


private:
    unsigned int last_frame_id;     //上一次检测的图片帧号
    int imageWidth;
    int imageHeight;
    QList<int> left_col_result;     //左列枣结果
    QList<int> right_col_result;    //右列枣结果

    QMap<QString, QVariant> dataResultMap;  //检测输出结果

    QFrameCalInfo frameCalInfo;
    QZaoInspectAlgParas zaoInspectAlgParas;
};

#endif // QZAOINSPECTALGAPP_H
