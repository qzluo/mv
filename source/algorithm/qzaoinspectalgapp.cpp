#include "qzaoinspectalgapp.h"
#include "cameraimagecache.h"

#include "Mat2QImage.h"
#include <FileLogger.h>

#include <QDebug>

QZaoInspectAlgApp::QZaoInspectAlgApp()
{
    imageWidth = 1280;
    imageHeight = 960;

    detectHandle = NULL;
    recognizeHandle = NULL;
}

QZaoInspectAlgApp::~QZaoInspectAlgApp()
{
    if (detectHandle) {
        HQEDestoryDetector(detectHandle);
        detectHandle = NULL;
    }

    if (recognizeHandle) {
        HQEDestoryRecognizer(recognizeHandle);
        recognizeHandle = NULL;
    }
}

int QZaoInspectAlgApp::init()
{
    if (loadCfgFile() < 0) {
        logFile(FileLogger::warn, "load alg cfg failed.");
        return -1;
    }

    detectHandle = HQECreateDetector(imageWidth, imageHeight);
    if (!detectHandle) {
        logFile(FileLogger::warn, "Create detector failed.");
        return -1;
    }

    recognizeHandle = HQECreateRecognizer();
    if (!recognizeHandle) {
        logFile(FileLogger::warn, "Create recognizer failed.");
        return -1;
    }

    //加载模型
    HQELoadModel(recognizeHandle,
                 QSysDefine::GetRecognizeModelFilePath().toLatin1().data(),
                 QSysDefine::GetRecognizeModelFileName().toLatin1().data());

    //set detect parameters and recognization parameters??????????????
    HQEDetectorParams detectParas = zaoInspectAlgParas.cerateDetectParas();
    HQESetParams(detectHandle, detectParas);

    //init 枣结果
    last_frame_id = 0;
    left_col_result.clear();
    right_col_result.clear();
    for (int i = 0; i < ZAO_REGION_COUNT; ++i) {
        left_col_result << ZAO_CLASS_NONE;
        right_col_result << ZAO_CLASS_NONE;
    }

    return 0;
}

int QZaoInspectAlgApp::resetImageSize(int imgWidth, int imgHeight)
{
    logFile(FileLogger::info, "resetImageSize");

    if (imgWidth <= 0 || imgHeight <= 0)
        return -1;

    imageWidth = imgWidth;
    imageHeight = imgHeight;

    char msg[1024] = {};
    sprintf(msg, "imageWidth = %d, imageHeight = %d",
            imageWidth, imageHeight);

    logFile(FileLogger::info, msg);

    if (detectHandle) {
        HQEDestoryDetector(detectHandle);
        detectHandle = NULL;
    }

    detectHandle = HQECreateDetector(imageWidth, imageHeight);
    HQEDetectorParams detectParas = zaoInspectAlgParas.cerateDetectParas();
    HQESetParams(detectHandle, detectParas);

    //修改帧间距离
    updateFrameDist();

    return 0;
}

void QZaoInspectAlgApp::updateFrameDist()
{
    //修改帧间距离
    int totalLength = 0;
    int frameDir = frameCalInfo.getFrameDir();
    if (frameDir == QFrameCalInfo::EFRAMEDIR_RIGHT ||
            frameDir == QFrameCalInfo::EFRAMEDIR_LEFT)
        totalLength = imageWidth;
    else if (frameDir == QFrameCalInfo::EFRAMEDIR_DOWN ||
             frameDir == QFrameCalInfo::EFRAMEDIR_UP)
        totalLength = imageHeight;

    double distBtFrms = double(totalLength) / ZAO_REGION_COUNT;
    if (distBtFrms != frameCalInfo.getDistBtFrms()) {
        frameCalInfo.setDistBtFrms(distBtFrms);
        frameCalInfo.save();
    }
}

int QZaoInspectAlgApp::reloadAlgParasFile(QString &fileName)
{
    if (zaoInspectAlgParas.loadFromFile(fileName) < 0)
        return -1;

    resetInspectParas();
    return 0;
}

int QZaoInspectAlgApp::saveAlgParas()
{
    return zaoInspectAlgParas.save();
}

int QZaoInspectAlgApp::saveAlgParasToFile(QString &fileName)
{
    return zaoInspectAlgParas.saveToFile(fileName);
}

void QZaoInspectAlgApp::resetInspectParas()
{
    if (!detectHandle)
        return;

    //set detect parameters and recognization parameters??????????????
    HQEDetectorParams detectParas = zaoInspectAlgParas.cerateDetectParas();
    HQESetParams(detectHandle, detectParas);
}

int QZaoInspectAlgApp::loadCfgFile()
{
    char msg[1024] = {};
    //init FrameFlowParas
    if (frameCalInfo.load() < 0) {
        logFile(FileLogger::warn, "Init frame flow paras failed!");
        return -1;
    }

    sprintf(msg, "frameDir = %d, factorMmToPix = %f, "
            "distBtFrms = %f",
            frameCalInfo.getFrameDir(),
            frameCalInfo.getFactorMmToPix(),
            frameCalInfo.getDistBtFrms());

    logFile(FileLogger::info, msg);

    //init GoodZaoClassThreshold
    zaoInspectAlgParas.load();

    sprintf(msg, "class_good1_length = %f, class_good1_width = %f, "
            "class_good2_length = %f, class_good2_width = %f, "
            "class_good3_length = %f, class_good3_width = %f, ",
            zaoInspectAlgParas.getClass_good1_length(),
            zaoInspectAlgParas.getClass_good1_width(),
            zaoInspectAlgParas.getClass_good2_length(),
            zaoInspectAlgParas.getClass_good2_width(),
            zaoInspectAlgParas.getClass_good3_length(),
            zaoInspectAlgParas.getClass_good3_width());

    logFile(FileLogger::info, msg);

    return 0;
}

int QZaoInspectAlgApp::inspect(const QImage &cameraImg, QImage &outImg)
{
    clear();

    //当前的检测图片帧号
    unsigned int cur_frame_id = cic->getCurFrameId();
    setDataVariant(getResDataDescFromId(E_Cur_FrameId),
                   QVariant(cur_frame_id));

    cv::Mat mat = ImageTransform::QImageToMat(cameraImg);

    //检测图片及识别种类
    QVector<ZaoInfo> vecZaoInfo;
    int zaoCount = 0;
    if (zaoInspect(mat, vecZaoInfo, &zaoCount) < 0) {
        logFile(FileLogger::warn, "inspect error!");
        setDataVariant(getResDataDescFromId(E_Inspect_Result),
                       QVariant(-1));
        return -1;
    }

    //生成当前检测结果，并分别放入左右队列中
    int regionCount = left_col_result.size();
    if (regionCount <= 0) {
        logFile(FileLogger::warn, "regionCount error!");
        setDataVariant(getResDataDescFromId(E_Inspect_Result),
                       QVariant(-1));
        return -1;
    }

    QList<int> cur_left_col_result;
    QList<int> cur_right_col_result;

    for (int i = 0; i < regionCount; ++i) {
        cur_left_col_result << ZAO_CLASS_NONE;
        cur_right_col_result << ZAO_CLASS_NONE;
    }

    for (int i = 0; i < zaoCount; ++i) {
        int leftOrRight = ZAO_COL_POS_LEFT;
        int regionId = 0;

        if (calcZaoRegionId(vecZaoInfo[i], &leftOrRight, &regionId) < 0) {
            logFile(FileLogger::warn, "calc zao calcZaoRegionId error!");
            setDataVariant(getResDataDescFromId(E_Inspect_Result),
                           QVariant(-1));
            return -1;
        }

        if (leftOrRight == ZAO_COL_POS_LEFT)
            cur_left_col_result[regionId] = vecZaoInfo[i].classId;
        else
            cur_right_col_result[regionId] = vecZaoInfo[i].classId;
    }

    //合并结果集
    int distBtFrameId = cur_frame_id - last_frame_id;
    for (int i = 0; i < regionCount; ++i) {
        int new_class = 0;
        if (i + distBtFrameId < regionCount) {
            mergeZaoClasses(left_col_result.at(i + distBtFrameId),
                            cur_left_col_result.at(i), &new_class);

            left_col_result[i] = new_class;

            mergeZaoClasses(right_col_result.at(i + distBtFrameId),
                            cur_right_col_result.at(i), &new_class);

            right_col_result[i] = new_class;
        }
        else {
            left_col_result[i] = cur_left_col_result.at(i);
            right_col_result[i] = cur_right_col_result.at(i);
        }
    }

    //更新帧序号
    last_frame_id = cur_frame_id;

    //保存结果
    setDataVariant(getResDataDescFromId(E_Inspect_Result),
                   QVariant(0));
    setDataVariant(getResDataDescFromId(E_Left_Col_Result),
                   QVariant(left_col_result[0]));
    setDataVariant(getResDataDescFromId(E_Right_Col_Result),
                   QVariant(right_col_result[0]));
    QVariant variant = QVariant::fromValue(vecZaoInfo);
    setDataVariant(getResDataDescFromId(E_Cur_Frame_Product_Info),
                   variant);

    return 0;
}

int QZaoInspectAlgApp::inspectSigleImage(const Mat &mat,
                                         QList<ZaoInfo> &cur_left_col_result,
                                         QList<ZaoInfo> &cur_right_col_result)
{
    //检测图片及识别种类
    QVector<ZaoInfo> vecZaoInfo;
    int zaoCount = 0;
    if (zaoInspect(mat, vecZaoInfo, &zaoCount) < 0) {
        logFile(FileLogger::warn, "inspect error!");
        return -1;
    }

    if (zaoCount > ZAO_REGION_COUNT * 2) {
        logFile(FileLogger::warn, "The count of found product is larger than expected!");
        return -1;
    }

    //生成当前检测结果，并分别放入左右队列中
    cur_left_col_result.clear();
    cur_right_col_result.clear();

    for (int i = 0; i < ZAO_REGION_COUNT; ++i) {
        ZaoInfo tmpZaoInfo = {};
        tmpZaoInfo.classId = ZAO_CLASS_NONE;
        cur_left_col_result << tmpZaoInfo;
        cur_right_col_result << tmpZaoInfo;
    }

    for (int i = 0; i < zaoCount; ++i) {
        int leftOrRight = ZAO_COL_POS_LEFT;
        int regionId = 0;

        if (calcZaoRegionId(vecZaoInfo[i], &leftOrRight, &regionId) < 0) {
            logFile(FileLogger::warn, "calc zao calcZaoRegionId error!");
            return -1;
        }

        if (leftOrRight == ZAO_COL_POS_LEFT)
            cur_left_col_result[regionId] = vecZaoInfo[i];
        else
            cur_right_col_result[regionId] = vecZaoInfo[i];
    }

    return 0;
}

int QZaoInspectAlgApp::getDataVariant(const QString &dataName, QVariant &var)
{
    var = dataResultMap.value(dataName, QVariant());
    return (var.isNull() ? -1 : 0);
}

void QZaoInspectAlgApp::setDataVariant(const QString &dataName, QVariant &var)
{
    dataResultMap.insert(dataName, var);
}

QString QZaoInspectAlgApp::getResDataDescFromId(QZaoInspectAlgApp::EResDataId eResDataId)
{
    switch (eResDataId) {
    case E_Inspect_Result:
        return QString("Inspect Result");

    case E_Cur_FrameId:
        return QString("Current FrameId");

    case E_Left_Col_Result:
        return QString("Left Column Result");

    case E_Right_Col_Result:
        return QString("Right Column Result");

    case E_Cur_Frame_Product_Info:
        return QString("Current Frame Product Infomation");

    default:
        break;
    }

    return QString("");
}

QFrameCalInfo *QZaoInspectAlgApp::getPFrameCalInfo()
{
    return &frameCalInfo;
}

QZaoInspectAlgParas *QZaoInspectAlgApp::getPInspectAlgParas()
{
    return &zaoInspectAlgParas;
}

void QZaoInspectAlgApp::clear()
{
    dataResultMap.clear();
}

//预定义枣区域数为4
int QZaoInspectAlgApp::calcRegionCount()
{
    return ZAO_REGION_COUNT;

    double distBtFrms = frameCalInfo.getDistBtFrms();
    if (distBtFrms <= 0)
        return -1;

    int totalLength = 0;
    int frameDir = frameCalInfo.getFrameDir();
    if (frameDir == QFrameCalInfo::EFRAMEDIR_RIGHT ||
            frameDir == QFrameCalInfo::EFRAMEDIR_LEFT)
        totalLength = imageWidth;
    else if (frameDir == QFrameCalInfo::EFRAMEDIR_DOWN ||
             frameDir == QFrameCalInfo::EFRAMEDIR_UP)
        totalLength = imageHeight;

    return ceil(totalLength / distBtFrms);
}

int QZaoInspectAlgApp::calctZaoClass(ZaoInfo zaoInfo, int *zaoClass)
{
    switch (zaoInfo.classId) {
    case ZAO_CLASS_GOOD1:
    case ZAO_CLASS_GOOD2:
    case ZAO_CLASS_GOOD3:
    case ZAO_CLASS_GOOD4:
    {
        //计算枣长宽
        int zaoLength = max(zaoInfo.zaoPos.width,
                           zaoInfo.zaoPos.height);
        int zaoWidth = min(zaoInfo.zaoPos.width,
                           zaoInfo.zaoPos.height);

        if (zaoLength >= zaoInspectAlgParas.getClass_good1_length() &&
                zaoWidth >= zaoInspectAlgParas.getClass_good1_width())
            *zaoClass = ZAO_CLASS_GOOD1;
        else if (zaoLength >= zaoInspectAlgParas.getClass_good2_length() &&
                 zaoWidth >= zaoInspectAlgParas.getClass_good2_width())
            *zaoClass = ZAO_CLASS_GOOD2;
        else if (zaoLength >= zaoInspectAlgParas.getClass_good3_length() &&
                 zaoWidth >= zaoInspectAlgParas.getClass_good3_width())
            *zaoClass = ZAO_CLASS_GOOD3;
        else
            *zaoClass = ZAO_CLASS_GOOD4;

        break;
    }

    case ZAO_CLASS_BAD1:
    case ZAO_CLASS_BAD2:
    case ZAO_CLASS_BAD3:
    case ZAO_CLASS_BAD4:
        *zaoClass = zaoInfo.classId;
        break;

    default:
        return -1;
    }

    return 0;
}

int QZaoInspectAlgApp::calcZaoRegionId(ZaoInfo zaoInfo, int *leftOrRight, int *regionId)
{
    //计算枣中心（x, y）
    double zaoCenterX = zaoInfo.zaoPos.x + zaoInfo.zaoPos.width / 2.0;
    double zaoCenterY = zaoInfo.zaoPos.y + zaoInfo.zaoPos.height / 2.0;

    int frameDir = frameCalInfo.getFrameDir();
    double distBtFrms = frameCalInfo.getDistBtFrms();
    if (frameDir == QFrameCalInfo::EFRAMEDIR_DOWN) {
        *leftOrRight = (zaoCenterX > imageWidth / 2.0) ?
                            ZAO_COL_POS_LEFT : ZAO_COL_POS_RIGHT;

        double distFromEdge = imageHeight - zaoCenterY;
        Q_ASSERT(distFromEdge >= 0);

        *regionId = distFromEdge / distBtFrms;
    }
    else if (frameDir == QFrameCalInfo::EFRAMEDIR_RIGHT) {
        *leftOrRight = (zaoCenterY < imageHeight / 2.0) ?
                            ZAO_COL_POS_LEFT : ZAO_COL_POS_RIGHT;

        double distFromEdge = imageWidth - zaoCenterX;
        Q_ASSERT(distFromEdge >= 0);

        *regionId = distFromEdge / distBtFrms;
    }
    else if (frameDir == QFrameCalInfo::EFRAMEDIR_UP) {
        *leftOrRight = (zaoCenterX < imageWidth / 2.0) ?
                            ZAO_COL_POS_LEFT : ZAO_COL_POS_RIGHT;

        double distFromEdge = zaoCenterY;
        Q_ASSERT(distFromEdge >= 0);

        *regionId = distFromEdge / distBtFrms;
    }
    else if (frameDir == QFrameCalInfo::EFRAMEDIR_LEFT) {
        *leftOrRight = (zaoCenterY > imageHeight / 2.0) ?
                            ZAO_COL_POS_LEFT : ZAO_COL_POS_RIGHT;

        double distFromEdge = zaoCenterX;
        Q_ASSERT(distFromEdge >= 0);

        *regionId = distFromEdge / distBtFrms;
    }

    if (*regionId >= left_col_result.size())
        *regionId = left_col_result.size() - 1;

    return 0;
}

int QZaoInspectAlgApp::mergeZaoClasses(int class1, int class2, int *retClass)
{
    if (class1 == ZAO_CLASS_NONE || class2 == ZAO_CLASS_NONE)
        *retClass = min(class1, class2);
    else {
        if (max(class1, class2) >= ZAO_CLASS_BAD1)
            *retClass = max(class1, class2);
        else
            *retClass = min(class1, class2);
    }

    return 0;
}

int QZaoInspectAlgApp::zaoInspect(cv::Mat imageIn, QVector<ZaoInfo>& vecZaoInfo, int *zaoCount)
{
    try {
        if (!detectHandle || !recognizeHandle)
            return -1;

        char msg[1024] = {};

        //detection
        cv::Mat mask;
        std::vector<Rect> rect_vec;
        std::vector<cv::Mat>  img_roi_vec;
        HQEDetection(detectHandle, imageIn, mask, img_roi_vec, rect_vec);
        if (img_roi_vec.size() != rect_vec.size()) {
            sprintf(msg, "The count of img_roi_vec[%d] is"
                         " not equal to the count of rect_vec[%d].",
                    img_roi_vec.size(), rect_vec.size());
            logFile(FileLogger::warn, msg);

            return -1;
        }
        Q_ASSERT(img_roi_vec.size() == rect_vec.size());

        //recognition
        std::vector<int> label_vec;
        HQERecognization(recognizeHandle, img_roi_vec, label_vec);
        if (img_roi_vec.size() != label_vec.size()) {
            sprintf(msg, "The count of img_roi_vec[%d] is"
                         " not equal to the count of label_vec[%d].",
                    img_roi_vec.size(), label_vec.size());
            logFile(FileLogger::warn, msg);

            return -1;
        }
        Q_ASSERT(img_roi_vec.size() == label_vec.size());

        *zaoCount = (int)img_roi_vec.size();

        vecZaoInfo.clear();
        for (int i = 0; i < *zaoCount; ++i) {
            ZaoInfo zaoInfo = {};
            //label_vec[i]: 1-好;2-皮皮;3-破皮;4-裂痕;5-黑枣
            zaoInfo.classId = label_vec[i] + ZAO_CLASS_BAD1 - 2;
            zaoInfo.zaoPos = rect_vec[i];

            if (label_vec[i] == 1) { //好枣
                int zaoClass = ZAO_CLASS_GOOD1;
                if (calctZaoClass(zaoInfo, &zaoClass) < 0) {
                    logFile(FileLogger::warn, "calc zao class error!");
                    return -1;
                }

                zaoInfo.classId = zaoClass;
            }

            vecZaoInfo.append(zaoInfo);
        }
    }
    catch (...) {
        logFile(FileLogger::warn, "Error in jujube inspect.");
        return -1;
    }

    return 0;
}
