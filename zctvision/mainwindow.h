#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "graphicswidget.h"

#include "mainresource.h"

#include "qzaoinspectalgapptester.h"

#include "vimbatester.h"
#include "qrealtimeresultwidget.h"

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

    //显示系统状态
    void showStatus();

private:
    void onSystemStateStarted();
    void onSystemStateStopped();
    void onInspectStateStarted();
    void onInspectStateStopped();

    void hasNextAndLastFile(QString& fullFileName, bool* pHasNext, bool* bHasLast);
    bool hasNextFile(QString& fullFileName);

    QString getNextOrLastFileName(QString& fullFileName, bool bNext);
    void onNextOrLastImageBtnClicked(bool bNext);

    void testHasNextFile();
    void testHasNextAndLastFile(void);

public slots:
    void onHasImage(const QImage& image);
    void onInspectDone(DetectResult result);
    void onLoginBtnClicked();
    void onOpenCfgFileBtnClicked();
    void onSaveCfgFileBtnClicked();
    void onLoadAlgParasFileBtnClicked();
    void onSaveAlgParasBtnClicked();
    void onSaveAlgParasToFileBtnClicked();
    void onUpdateAlgParasOnNetworkBtnClicked();
    void onSelCamTypeBtnClicked();
    void onCommSetupBtnClicked();
    void onSysParasSetupBtnClicked();
    void onStartSysBtnClicked();
    void onStartInspectBtnClicked();
    void onOpenImageBtnClicked();
    void onNextImageBtnClicked();
    void onLastImageBtnClicked();
    void onRunOnceBtnClicked();
    void onRecognizeParasBtnClicked();
    void onGradeParasBtnClicked();
    void onFrameCalActionTriggered();
    void onAlgParasActionTriggered();
    void onCamParasBtnClicked();
    void onModbusCmdActionTriggered();
    void onStaticsInfosToolBtnClicked();
    void onAboutToolBtnClicked();

private:
    Ui::MainWindow *ui;

    QToolButton *loginToolBtn;
    QToolButton *openCfgFileToolBtn;
    QToolButton *saveCfgFileToolBtn;
    QToolButton *loadAlgParasFileToolBtn;
    QToolButton *saveAlgParasToolBtn;
    QToolButton *saveAlgParasToFileToolBtn;
    QToolButton *updateAlgParasOnNetwork;

    QToolButton *selCamTypeToolBtn;
    QToolButton *commSetupToolBtn;
    QToolButton *sysParasToolBtn;
    QToolButton *startSysToolBtn;
    QToolButton *startInspectToolBtn;

    QToolButton *openImageToolBtn;
    QToolButton *nextImageToolBtn;
    QToolButton *lastImageToolBtn;

    QToolButton *cameraParasToolBtn;
    QToolButton *ctlboardParasToolBtn;
    QToolButton *staticsInfosToolBtn;

    QToolButton* runOnceBtn;
    QPushButton* recognizeParasBtn;
    QPushButton* gradeParasBtn;

//    QToolButton *setupToolBtn;
//    QAction* modbusCmdAction;

    QRealTimeResultWidget* rtResultWidget;

    QTextBrowser* resultTB;

    GraphicsWidget* plot;

    //状态栏
    QLabel* statusInfo;

    QImage curImage;    //当前待检测的图片
    QString curImageFileName;
    bool hasNext;
    bool hasPrev;
};

#endif // MAINWINDOW_H
