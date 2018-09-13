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

#include "HQEDetector.h"
#include "HQERecognizer.h"

#define ZAO_REGION_COUNT 4  //一帧图片包含的最多枣的排数

typedef struct tagDetectResult {
    int result;
    int frameId;
    int left_col_grade_result;
    int right_col_grade_result;
    QVariant curFrameZaoInfo;
} DetectResult, *PDetectResult;

//枣信息
typedef struct tagZaoInfo {
    int classId;    //等级
    cv::Rect zaoPos;
} ZaoInfo, *PZaoInfo;

Q_DECLARE_METATYPE(ZaoInfo)


///
/// \brief The QZaoInspectAlgApp class
/// 1. 检测当前帧的枣信息
/// 2. 处理当前帧的枣信息，包括将好枣根据设置的阈值分类及将枣归入实现划分的区域里
/// 3. 那些区域是为了综合多次枣信息，只传出下一帧将要移出视野的枣信息
/// 4. 综合枣信息需要考虑之前处理的帧号，及当前检测的帧号，枣在两个连续帧间移动一个划分区域
/// 5. 根据设定，每帧图片最多有4排枣
class QZaoInspectAlgApp
{
public:
    QZaoInspectAlgApp();

    ~QZaoInspectAlgApp();

    enum EResDataId {
        E_Inspect_Result = 0,
        E_Cur_FrameId,
        E_Cur_Frame_Result_info,
        E_Left_Grade_Result_ToSend,
        E_Right_Grade_Result_ToSend,
    };

    //枣所属列
    enum EZAO_COL_POS {
        ZAO_COL_POS_LEFT = 0,
        ZAO_COL_POS_RIGHT,
    };

    int init(void);   //系统启动时初始化

    //相机启动后，需重置图像尺寸
    int resetImageSize(int imgWidth, int imgHeight);

    //帧校正信息改变
    void updateFrameDist(void);

    //加载算法参数配置文件,如加载成功，需重置检测参数
    int reloadAlgParasFile(QString& fileName);

    //保存算法参数到默认的文件
    int saveAlgParas();

    //保存算法参数到指定的文件
    int saveAlgParasToFile(QString& fileName);

    //修改检测参数后，需调用
    void resetInspectParas();

    //检测，需综合多张图片的结果
    int inspect(const QImage &cameraImg, QImage& outImg = QImage());

    ///
    /// \brief inspectSigleImage 检测单张图片
    /// \param mat: 输入图片
    /// \param left_col_result: 左列结果
    /// \param right_col_result: 右列结果
    /// \return
    ///
    int inspectSingleImage(const cv::Mat& mat,
                          QList<ZaoInfo>& cur_left_col_result,
                          QList<ZaoInfo>& cur_right_col_result);

    int inspectSingleImage(const QImage& image,
                          QList<ZaoInfo>& cur_left_col_result,
                          QList<ZaoInfo>& cur_right_col_result);

    //获取指定的检测结果
    int getDataVariant(const QString& dataName, QVariant& var);

    //更新检测结果
    void setDataVariant(const QString& dataName, QVariant& var);

    static QString getResDataDescFromId(EResDataId eResDataId);

    QFrameCalInfo* getPFrameCalInfo(void);
    QZaoInspectAlgParas* getPInspectAlgParas(void);


//private:

    int loadCfgFile(void);
    void clear(void);

    //计算区域数，将图片分割成一条条带状，物料将依次在带中移动
    int calcRegionCount();

    //计算枣等级，主要是区分出好枣的等级
    int calctZaoClass(ZaoInfo zaoInfo, int* zaoClass);

    //计算枣所属的区域Id
    int calcZaoRegionId(ZaoInfo zaoInfo, int* leftOrRight, int* regionId);

    //综合枣等级
    int mergeZaoClasses(int class1, int class2, int* retClass);

    ///
    /// \brief zaoInspect: 枣检测及识别算法接口
    /// \param imageIn -- [in]待检测图片
    /// \param vecZaoInfo -- [out]图片中识别的枣信息
    /// \param zaoCount -- [out]共识别出的枣个数
    /// \return 0 -- 无异常， -1 -- 异常，错误
    ///
    int zaoInspect(cv::Mat imageIn, QVector<ZaoInfo>& vecZaoInfo, int *zaoCount);

private:
    unsigned int last_frame_id;     //上一次检测的图片帧号
    int imageWidth;
    int imageHeight;
    QList<int> left_col_result;     //左列枣结果
    QList<int> right_col_result;    //右列枣结果

    ///
    /// \brief dataResultMap
    /// 检测输出的结果，包含如下信息
    /// 1. 检测是否有错
    /// 2. 当前图片帧号
    /// 3. 当前图片中各单元格的检测信息，前面4格放左列信息，后面4格放右列信息
    /// 4. 本次检测后，综合需要输出的左列结果
    /// 5. 本次检测后，综合需要输出的右列结果
    QMap<QString, QVariant> dataResultMap;  //检测输出结果

    QFrameCalInfo frameCalInfo;
    QZaoInspectAlgParas zaoInspectAlgParas;

    void* detectHandle;
    void* recognizeHandle;
};

#endif // QZAOINSPECTALGAPP_H
