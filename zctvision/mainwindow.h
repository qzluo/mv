#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "graphicswidget.h"

#include "mainresource.h"

#include "qzaoinspectalgapptester.h"

#include "vimbatester.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init(void);

private:
    void onSystemStateStarted();
    void onSystemStateStopped();
    void onInspectStateStarted();
    void onInspectStateStopped();

public slots:
    void onHasImage(const QImage& image);
    void onInspectDone(DetectResult result);
    void onOpenCfgFileBtnClicked();
    void onSaveCfgFileBtnClicked();
    void onLoadAlgParasFileBtnClicked();
    void onSaveAlgParasBtnClicked();
    void onSaveAlgParasToFileBtnClicked();
    void onSelCamTypeBtnClicked();
    void onCommSetupBtnClicked();
    void onStartSysBtnClicked();
    void onStartInspectBtnClicked();
    void onFrameCalActionTriggered();
    void onAlgParasActionTriggered();
    void onModbusCmdActionTriggered();

private:
    Ui::MainWindow *ui;

    QToolButton *openCfgFileToolBtn;
    QToolButton *saveCfgFileToolBtn;
    QToolButton *loadAlgParasFileToolBtn;
    QToolButton *saveAlgParasToolBtn;
    QToolButton *saveAlgParasToFileToolBtn;

    QToolButton *selCamTypeToolBtn;
    QToolButton *commSetupToolBtn;
    QToolButton *startSysToolBtn;
    QToolButton *startInspectToolBtn;

    QToolButton *setupToolBtn;
    QAction* modbusCmdAction;

    QTextBrowser* resultTB;

//    MainResource rc;
    GraphicsWidget* plot;

    QZaoInspectAlgAppTester tester;
};

#endif // MAINWINDOW_H
