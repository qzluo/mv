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
    void startNewInspect(void);
    QImage getLastImage();
    unsigned int getCurFrameId(void);

    void updateFrameId(unsigned int frameId);
    void setImageSize(int width, int height);
    void getImageSize(int* width, int* height);

signals:
    void hasNewImage();

public slots:
    void onHasImage(const QImage& image);

private:
    static CameraImageCache* instance;

    QMutex mutex;
    QImage lastImage;         //最后一张图片
    unsigned int frameId;     //最新的图片帧号
    unsigned int inspectFrameId;     //检测的图片帧号

    bool needInspect;

    int imageWidth;
    int imageHeight;
};

#endif // CAMERAIMAGECACHE_H
