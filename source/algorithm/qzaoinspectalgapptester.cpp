#include "qzaoinspectalgapptester.h"

#include <QDebug>


QZaoInspectAlgAppTester::QZaoInspectAlgAppTester(QObject *parent) : QObject(parent)
{

}

void QZaoInspectAlgAppTester::testInit()
{
    qDebug() << "QZaoInspectAlgAppTester::testInit()";


    qDebug() << "region count: " << inst.calcRegionCount();
}

int QZaoInspectAlgAppTester::testCalctZaoClass()
{
    qDebug() << "QZaoInspectAlgAppTester::testCalctZaoClass()";

    ZaoInfo zaoInfo = {};
    int zaoClass = 0;
    int ret = 0;

    //ZAO_CLASS_BAD
    for (int i = ZAO_CLASS_BAD1; i < ZAO_CLASS_BAD4 + 1; ++i) {
        zaoInfo.classId = i;
        ret = inst.calctZaoClass(zaoInfo, &zaoClass);

        if (ret < 0 || zaoClass != i) {
            qDebug() << QString("ret = %1, expected class = %2,"
                                " return class = %3").arg(ret).
                        arg(i).arg(zaoClass);
            return -1;
        }
    }

    //ZAO_CLASS_GOOD1
    zaoInfo.classId = ZAO_CLASS_GOOD1;
    zaoInfo.zaoPos = cv::Rect(0, 0, 10, 5);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD1) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD1).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 12, 2);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD1) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD1).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 6, 5);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD1) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD1).arg(zaoClass);
        return -1;
    }

    //ZAO_CLASS_GOOD2
    zaoInfo.classId = ZAO_CLASS_GOOD2;
    zaoInfo.zaoPos = cv::Rect(0, 0, 8, 4);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD2) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD2).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 9, 2);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD2) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD2).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 4, 4);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD2) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD2).arg(zaoClass);
        return -1;
    }

    //ZAO_CLASS_GOOD2
    zaoInfo.classId = ZAO_CLASS_GOOD3;
    zaoInfo.zaoPos = cv::Rect(0, 0, 6, 3);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD3) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD3).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 7, 2);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD3) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD3).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 3, 3);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD3) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD3).arg(zaoClass);
        return -1;
    }

    zaoInfo.zaoPos = cv::Rect(0, 0, 2, 2);
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    if (ret < 0 || zaoClass != ZAO_CLASS_GOOD4) {
        qDebug() << QString("ret = %1, expected class = %2,"
                        " return class = %3").arg(ret).
                    arg(ZAO_CLASS_GOOD4).arg(zaoClass);
        return -1;
    }

    zaoInfo.classId = ZAO_CLASS_NONE;
    ret = inst.calctZaoClass(zaoInfo, &zaoClass);
    qDebug() << QString("ret = %1, expected class = %2,"
                    " return class = %3").arg(ret).
                arg(ZAO_CLASS_GOOD4).arg(zaoClass);

    return 0;
}

int QZaoInspectAlgAppTester::testCalcZaoRegionId()
{
    qDebug() << "QZaoInspectAlgAppTester::testCalcZaoRegionId()";

    ZaoInfo zaoInfo = {};
    int leftOrRight = 0;
    int regionId = 0;
    int ret = 0;

    int centerX = 0;
    int centerY = 0;
    int width = 10;
    int height = 6;

    for (int i = 0; i < 10; ++i) {
        centerX = qrand() % 1280;
        centerY = qrand() % 960;
        zaoInfo.zaoPos = cv::Rect(centerX - 5, centerY - 3, width, height);

        int expectedLeftOrRight = (centerX > 1280 / 2) ?
                    QZaoInspectAlgApp::ZAO_COL_POS_LEFT :
                    QZaoInspectAlgApp::ZAO_COL_POS_RIGHT;
        int expectedRegionId = (960 - centerY) / 100;

        ret = inst.calcZaoRegionId(zaoInfo, &leftOrRight, &regionId);

        qDebug() << QString("ret = %1, expectedLeftOrRight = %2,"
                        " expectedRegionId = %3,"
                     "return leftOrRight = %4, return regionId = %5").arg(ret).
                    arg(expectedLeftOrRight).arg(expectedRegionId).
                    arg(leftOrRight).arg(regionId);
    }

    return 0;
}

int QZaoInspectAlgAppTester::testZaoInspect()
{
    qDebug() << "QZaoInspectAlgAppTester::testZaoInspect()";

//    char* imageName = "E:\\project\\test\\test\\images\\Camera GED130C#C0A800EE-Snapshot-20180407103104-12346905765.bmp";
//    Mat img;
//    img = imread(imageName, 1);
//    QVector<ZaoInfo> vecZaoInfo;
//    int zaoCount = 0;

//    int ret = inst.zaoInspect(img, vecZaoInfo, &zaoCount);

//    if (ret < 0) {
//        qDebug() << "zao inspect failed.";
//        return -1;
//    }

//    qDebug() << "zao count: " << zaoCount;
//    for (int i = 0; i < vecZaoInfo.size(); ++i) {
//        qDebug() << vecZaoInfo[i].classId;
//        rectangle(img, Point(vecZaoInfo[i].zaoPos.x, vecZaoInfo[i].zaoPos.y),
//                  Point(vecZaoInfo[i].zaoPos.x + vecZaoInfo[i].zaoPos.width,
//                        vecZaoInfo[i].zaoPos.y + vecZaoInfo[i].zaoPos.height),
//                  Scalar(0, 255, 0), 2, 8);
//    }

//    namedWindow("result", WINDOW_AUTOSIZE);
//    imshow("result", img);
//    waitKey(0);

    return 0;
}

int QZaoInspectAlgAppTester::testInspect()
{
    qDebug() << "QZaoInspectAlgAppTester::testInspect()";


    inst.inspect(QImage());

    QVariant var;
//    inst.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
//                            QZaoInspectAlgApp::E_Cur_Frame_Product_Info),
//                        var);

//    QList<ZaoInfo> cur_zao_infos = var.value<QList<ZaoInfo>>();

//    qDebug() << "zao count: " << cur_zao_infos.size();

//    for (int i = 0; i < cur_zao_infos.size(); ++i) {
//        ZaoInfo zao_info = cur_zao_infos[i];
//        qDebug() << QString("zao class id = %1").arg(zao_info.classId);
//        qDebug() << QString("zao rect: [%1, %2, %3, %4]").
//                    arg(zao_info.zaoPos.x).arg(zao_info.zaoPos.y).
//                    arg(zao_info.zaoPos.width).arg(zao_info.zaoPos.height);

//    }

//    inst.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
//                            QZaoInspectAlgApp::E_Cur_FrameId),
//                        var);
//    qDebug() << "current FrameId: " << var.toUInt();

//    inst.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
//                            QZaoInspectAlgApp::E_Inspect_Result),
//                        var);
//    qDebug() << "inspect return: " << var.toInt();

//    inst.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
//                            QZaoInspectAlgApp::E_Left_Col_Result),
//                        var);
//    qDebug() << "E_Left_Col_Result: " << var.toInt();

//    inst.getDataVariant(QZaoInspectAlgApp::getResDataDescFromId(
//                            QZaoInspectAlgApp::E_Right_Col_Result),
//                        var);
    qDebug() << "E_Right_Col_Result: " << var.toInt();

    return 0;
}

int QZaoInspectAlgAppTester::testInspectSigleImage()
{
    qDebug() << "QZaoInspectAlgAppTester::testInspectSigleImage()";

    inst.init();

    const char* imageName = "F:\\VirCam\\TmpImage3\\Camera GED130C#C0A800EE-Snapshot-20180407103104-12346905765.BMP";
    Mat img;
    img = imread(imageName, 1);

    qDebug() << "width: " << img.cols;
    qDebug() << "height: " << img.rows;
    inst.resetImageSize(img.cols, img.rows);

    QList<ZaoInfo> cur_left_col_result;
    QList<ZaoInfo> cur_right_col_result;
    if (inst.inspectSingleImage(img, cur_left_col_result, cur_right_col_result) < 0)
        return -1;

    for (int i = 0; i < cur_left_col_result.size(); ++i) {
        qDebug() << cur_left_col_result[i].classId;
        rectangle(img, Point(cur_left_col_result[i].zaoPos.x, cur_left_col_result[i].zaoPos.y),
                  Point(cur_left_col_result[i].zaoPos.x + cur_left_col_result[i].zaoPos.width,
                        cur_left_col_result[i].zaoPos.y + cur_left_col_result[i].zaoPos.height),
                  Scalar(0, 255, 0), 2, 8);

        qDebug() << cur_right_col_result[i].classId;
        rectangle(img, Point(cur_right_col_result[i].zaoPos.x, cur_right_col_result[i].zaoPos.y),
                  Point(cur_right_col_result[i].zaoPos.x + cur_right_col_result[i].zaoPos.width,
                        cur_right_col_result[i].zaoPos.y + cur_right_col_result[i].zaoPos.height),
                  Scalar(0, 255, 0), 2, 8);
    }

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", img);
    waitKey(0);

    return 0;
}
