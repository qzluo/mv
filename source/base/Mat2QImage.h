#ifndef MAT2QIMAGE_H
#define MAT2QIMAGE_H

#include "cv.h"
#include <QImage>
using namespace cv;

class ImageTransform

{
public:
    ImageTransform();
    ~ImageTransform();
    //static QImage ConvertToQImage(cv::Mat &mat);

    static QImage MatToQImage(const Mat &mat,bool bClone = false);
    static Mat QImageToMat(const QImage &image,bool bClone = false);

    static QString QImageToText(const QImage &image, const QString &imageFormat, int quality = -1);
    static QImage TextToQImage(const QString &imageText);

    static QString MatToText(const Mat &image, const QString &imageFormat, int quality = -1);
    static Mat TextToMat(const QString &imageText);
};

#endif // MAT2QIMAGE_H
