#ifndef QIMAGEVIEWERWIDGET_H
#define QIMAGEVIEWERWIDGET_H

#include <QtWidgets>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class GraphicsWidget;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(GraphicsWidget *v) : QGraphicsView(), view(v) { }

protected:
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
#endif

private:
    GraphicsWidget *view;
};


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QObject *parent = 0);
    ~GraphicsScene(void);
    void setImage(const QPixmap &pixmap);

private:
    QGraphicsPixmapItem* imageItem;
};


class GraphicsWidget : public QFrame
{
    Q_OBJECT
public:
    explicit GraphicsWidget(QWidget *parent = 0);

    QGraphicsView *view() const;

    void setImage(const QPixmap &pixmap);

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();

private:
    GraphicsScene* scene;
    GraphicsView *graphicsView;

    QToolButton *resetButton;
    QSlider *zoomSlider;

};

#endif // QIMAGEVIEWERWIDGET_H
