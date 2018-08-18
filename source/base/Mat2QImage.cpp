#include "Mat2QImage.h"

#include <QBuffer>
#include <QImageReader>
#include <QDebug>

ImageTransform::ImageTransform
()
{

}

ImageTransform::~ImageTransform
()
{

}

////Mat->QImage
//QImage Mat2QImage::ConvertToQImage(cv::Mat &mat)
//{
//    QImage img;
//    int nChannel=mat.channels();
//    if(nChannel==3)
//    {
//        cv::cvtColor(mat,mat,CV_BGR2RGB);
//        img = QImage((const unsigned char*)mat.data,
//                     mat.cols,mat.rows,
//                     mat.cols*mat.channels(),
//                     QImage::Format_RGB888);
//    }
//    else if(nChannel==1)
//    {
//        img = QImage((const unsigned char*)mat.data,
//                     mat.cols,mat.rows,
//                     mat.cols*mat.channels(),
//                     QImage::Format_Indexed8);
//    }
//    return img;
//}

//��ʽ����
//QImage Mat2QImage::MatToQImage(const Mat& mat)
//{
//    // 8-bits unsigned, NO. OF CHANNELS=1
//    if(mat.type()==CV_8UC1)
//    {
//        // Set the color table (used to translate colour indexes to qRgb values)
//        QVector<QRgb> colorTable;
//        for (int i=0; i<256; i++)
//            colorTable.push_back(qRgb(i,i,i));
//        // Copy input Mat
//        const uchar *qImageBuffer = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
//        img.setColorTable(colorTable);
//        return img;
//    }
//    // 8-bits unsigned, NO. OF CHANNELS=3
//    if(mat.type()==CV_8UC3)
//    {
//        // Copy input Mat
//        const uchar *qImageBuffer = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
//        return img.rgbSwapped();
//    }
//    else
//    {
//        qDebug() << "ERROR: Mat could not be converted to QImage.";
//        return QImage();
//    }
//}

QImage ImageTransform::MatToQImage(const Mat& mat, bool bClone)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
//        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
//        // Set the color table (used to translate colour indexes to qRgb values)
//        image.setColorCount(256);
//        for(int i = 0; i < 256; i++)
//        {
//            image.setColor(i, qRgb(i, i, i));
//        }
//        // Copy input Mat
//        uchar *pSrc = mat.data;
//        for(int row = 0; row < mat.rows; row ++)
//        {
//            uchar *pDest = image.scanLine(row);
//            memcpy(pDest, pSrc, mat.cols);
//            pSrc += mat.step;
//        }
//        return image;

        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(colorTable);
        if(bClone)
        {
            return image.copy();
        }
        else
        {
            return image;
        }
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        if(bClone)
        {
            return image.rgbSwapped().copy();
        }
        else
        {
            return image.rgbSwapped();
        }
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "MatToQImage:CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        if(bClone)
        {
            return image.copy();
        }
        else
        {
            return image;
        }
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

Mat ImageTransform::QImageToMat(const QImage &image,bool bClone)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
    {
        QImage grayImg = image.convertToFormat(QImage::Format_Indexed8);
        mat = cv::Mat(grayImg.height(), grayImg.width(), CV_8UC1, (void*)grayImg.constBits(), grayImg.bytesPerLine()).clone();
        break;
    }
    }
    if(bClone)
    {
        return mat.clone();
    }
    else
    {
        return mat;
    }
}

QString ImageTransform::QImageToText(const QImage &image, const QString &imageFormat,int quality)
{
    QByteArray Data;
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    //qint64(0)��ʾ��0ת����qint64����,��(qint64)0�ȼ�
    //��������С��ͼƬ��ʽ��ͼƬ��С��Ϣ
    out   << qint64(0)<<imageFormat;
    //�����ڷ���һ��ͼƬʱ�����Ƚ� ͼƬ��С��Ϣ д����������������ڽ� JPG��ʽͼƬд�� ��������������ȷ���� ��������ȷ����һ������ͼƬ��
    QBuffer buffer;
    image.save(&buffer,imageFormat.toLatin1(),quality);
    out<<(quint32)buffer.data().size();
    Data.append(buffer.data());
    //sendOut.device()Ϊ����io�豸�ĵ�ǰ���ã�seek(0)��ʾ���õ�ǰposΪ0
    out.device()->seek(0);//���ص�outBlock�Ŀ�ʼ��ִ�и��ǲ���
    //�����ܴ�С�ռ���ļ�����С�ռ�
    out << qint64(Data.size());
    qDebug()<<"QImageToText:"<<Data.size()/1024<<"KB"<<Data.size()/1024/1024<<"MB";

    return QString(Data.toBase64());
}

QImage ImageTransform::TextToQImage(const QString &imageText)
{
    QByteArray data = QByteArray::fromBase64(imageText.toUtf8());

    QBuffer buffer(&data);
    buffer.open( QIODevice::ReadOnly);
    QDataStream in(&buffer);
    in.setVersion( QDataStream::Qt_4_6);
    //��������С��ͼƬ��ʽ��ͼƬ��С��Ϣ
    qint64 revDatasize(0);
    QString imageFormat;
    quint32 imageSize;
    in>>revDatasize>>imageFormat >>imageSize;

    QByteArray imageData = buffer.read(imageSize);
    QBuffer imageBuf(&imageData);
    imageBuf.open( QIODevice::ReadOnly);
    QImageReader reader(&imageBuf, imageFormat.toLatin1());
    QImage image = reader.read();
    return image;
}

QString ImageTransform::MatToText(const Mat &image, const QString &imageFormat, int quality)
{
    return QImageToText(MatToQImage(image),imageFormat, quality);
}

Mat ImageTransform::TextToMat(const QString &imageText)
{
    return QImageToMat(TextToQImage(imageText),true);
}

