#ifndef CAMERAIMAGECACHE_H
#define CAMERAIMAGECACHE_H

#include <QObject>
#include <QImage>
#include <QList>
#include <QMutex>

#define cic CameraImageCache::getInstance()

class CameraImageCache : public QObject
{
    Q_OBJECT

public:
    static CameraImageCache* getInstance();

private:
    explicit CameraImageCache(QObject *parent = 0);

public:
    void startNewCache(int cacheNum);//cacheNum为0时，不缓存也不会发送cacheFull
    QImage getLastImage();
    unsigned int getCurFrameId(void);

    void updateFrameId(unsigned int frameId);
    void setImageSize(int width, int height);
    void getImageSize(int* width, int* height);

signals:
    void cacheFull();

public slots:
    void onHasImage(const QImage& image);

private:
    static CameraImageCache* instance;

    QList<QImage> imageList;  //缓存的图片列表
    QMutex mutex;
    int cacheMaxNum;          //缓存的图片张数
    QImage lastImage;         //最后一张图片
    unsigned int frameId;     //最新的图片帧号
    unsigned int inspectFrameId;     //检测的图片帧号

    int imageWidth;
    int imageHeight;
};

#endif // CAMERAIMAGECACHE_H
