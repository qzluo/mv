#include "graphicswidget.h"

#include <qmath.h>

#ifndef QT_NO_WHEELEVENT
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            view->zoomIn(6);
        else
            view->zoomOut(6);
        e->accept();
    }
    else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif


GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    imageItem = NULL;
}

GraphicsScene::~GraphicsScene()
{
    if (imageItem) {
        delete imageItem;
        imageItem = NULL;
    }
}

void GraphicsScene::setImage(const QPixmap &pixmap)
{
    if (imageItem)
        imageItem->setPixmap(pixmap);
    else {
        imageItem = addPixmap(pixmap);
        imageItem->setZValue(-10);
    }
}

GraphicsWidget::GraphicsWidget(QWidget *parent) : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);

    //scene
    scene = new GraphicsScene(this);

    //view of scene
    graphicsView = new GraphicsView(this);
    graphicsView->setScene(scene);

    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/images/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/images/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setOrientation(Qt::Horizontal);

    resetButton = new QToolButton;
    resetButton->setText("0");
    resetButton->setEnabled(false);

    // Zoom slider layout
    QHBoxLayout *zoomSliderLayout = new QHBoxLayout;
    zoomSliderLayout->addWidget(zoomOutIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(resetButton);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(graphicsView, 1);
    topLayout->addLayout(zoomSliderLayout);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(setResetButtonEnabled()));

    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));

    setupMatrix();
}

QGraphicsView *GraphicsWidget::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void GraphicsWidget::setImage(const QPixmap &pixmap)
{
    scene->setImage(pixmap);
}

void GraphicsWidget::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void GraphicsWidget::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void GraphicsWidget::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);

    graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
}

void GraphicsWidget::resetView()
{
    zoomSlider->setValue(250);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void GraphicsWidget::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}
