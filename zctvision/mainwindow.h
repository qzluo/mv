#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "graphicswidget.h"

#include "mainresource.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onHasImage(const QImage& image);
    void onInspectDone(DetectResult result);
    void onSelCamTypeBtnClicked();
    void onStartSysBtnClicked();
    void onStartInspectBtnClicked();
    void onFrameCalActionTriggered();
    void onAlgParasActionTriggered();

private:
    Ui::MainWindow *ui;

    QToolButton *selCamTypeToolBtn;
    QToolButton *startSysToolBtn;
    QToolButton *startInspectToolBtn;

    QToolButton *setupToolBtn;

    QLineEdit* resultLE;

    MainResource rc;
    GraphicsWidget* plot;
};

#endif // MAINWINDOW_H
