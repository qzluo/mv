#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

#include "FileLogger.h"
#include "qselectcameratypedlg.h"
#include "qcommsetupdlg.h"
#include "qframecalinfosetupdlg.h"
#include "qalgparassetupdlg.h"
#include "qrtuoperatordlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //init UI
    //select camera type
    selCamTypeToolBtn = new QToolButton(this);
    selCamTypeToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    selCamTypeToolBtn->setIcon(QPixmap(":/images/camera.png"));
    selCamTypeToolBtn->setText(tr("Select Camera Type"));
    connect(selCamTypeToolBtn, &QToolButton::clicked,
            this, &MainWindow::onSelCamTypeBtnClicked);

    //comm setup
    commSetupToolBtn = new QToolButton(this);
    commSetupToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    commSetupToolBtn->setIcon(QPixmap(":/images/serialcomm.png"));
    commSetupToolBtn->setText(tr("Serial Setup"));
    connect(commSetupToolBtn, &QToolButton::clicked,
            this, &MainWindow::onCommSetupBtnClicked);

    //start system
    startSysToolBtn = new QToolButton(this);
    startSysToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    startSysToolBtn->setIcon(QPixmap(":/images/start_sys.png"));
    startSysToolBtn->setText(tr("Start System"));
    connect(startSysToolBtn, &QToolButton::clicked,
            this, &MainWindow::onStartSysBtnClicked);

    //start inspect
    startInspectToolBtn = new QToolButton(this);
    startInspectToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
    startInspectToolBtn->setText(tr("Start Inspect"));
    startInspectToolBtn->setEnabled(false);
    connect(startInspectToolBtn, &QToolButton::clicked,
            this, &MainWindow::onStartInspectBtnClicked);

    //setup
    setupToolBtn = new QToolButton(this);
    setupToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setupToolBtn->setIcon(QPixmap(":/images/tool_icon.png"));
    setupToolBtn->setText(tr("Setup"));

    QMenu* setupMenu = new QMenu(this);
    QAction* frameCalSetupAction = new QAction(QPixmap(":/images/calibration.png"),
                                               tr("Frame Calibration Setup"), this);
    QAction* algParasSetupAction = new QAction(QPixmap(":/images/parameter.png"),
                                               tr("Algorithm Parameters Setup"), this);
    modbusCmdAction = new QAction(QPixmap(":/images/modbus_cmd.png"),
                                               tr("Modbus Command"), this);
    connect(frameCalSetupAction, &QAction::triggered,
            this, &MainWindow::onFrameCalActionTriggered);
    connect(algParasSetupAction, &QAction::triggered,
            this, &MainWindow::onAlgParasActionTriggered);
    connect(modbusCmdAction, &QAction::triggered,
            this, &MainWindow::onModbusCmdActionTriggered);

    setupMenu->addAction(frameCalSetupAction);
    setupMenu->addAction(algParasSetupAction);
    setupMenu->addAction(modbusCmdAction);

    setupToolBtn->setMenu(setupMenu);
    setupToolBtn->setPopupMode(QToolButton::MenuButtonPopup);

    ui->mainToolBar->addWidget(selCamTypeToolBtn);
    ui->mainToolBar->addWidget(commSetupToolBtn);
    ui->mainToolBar->addWidget(startSysToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(startInspectToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(setupToolBtn);

    onSystemStateStopped();

    qDebug() << "Current currentThreadId" << QThread::currentThreadId();

    plot = new GraphicsWidget(this);

    QLabel* resultLabel = new QLabel(tr("Inspect Result:"), this);
    resultTB = new QTextBrowser(this);

    QVBoxLayout* btnLayout = new QVBoxLayout;
    btnLayout->addWidget(resultLabel);
    btnLayout->addWidget(resultTB);

    QWidget* mainWidget = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(mainWidget);
    topLayout->addWidget(plot, 1);
    topLayout->addLayout(btnLayout);

    setCentralWidget(mainWidget);

    setWindowState(this->windowState() ^ Qt::WindowMaximized);
    setWindowTitle(tr("Jujube Inspect Program"));

//    tester.test();

//    tester.testInit();
//    tester.testZaoInspect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    mic->init();

    //connect
    connect(mic, &MainResource::hasImage,
            this, &MainWindow::onHasImage);

    connect(mic, &MainResource::inspectDone,
            this, &MainWindow::onInspectDone);
}

void MainWindow::onSystemStateStarted()
{
    selCamTypeToolBtn->setEnabled(false);
    commSetupToolBtn->setEnabled(false);

    startSysToolBtn->setIcon(QPixmap(":/images/stop_sys.png"));
    startSysToolBtn->setText(tr("Stop System"));

    startInspectToolBtn->setEnabled(true);
    modbusCmdAction->setEnabled(true);
}

void MainWindow::onSystemStateStopped()
{
    selCamTypeToolBtn->setEnabled(true);
    commSetupToolBtn->setEnabled(true);
    startSysToolBtn->setIcon(QPixmap(":/images/start_sys.png"));
    startSysToolBtn->setText(tr("Start System"));

    startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
    startInspectToolBtn->setText(tr("Start Inspect"));
    startInspectToolBtn->setEnabled(false);

    modbusCmdAction->setEnabled(false);
    setupToolBtn->setEnabled(true);
}

void MainWindow::onInspectStateStarted()
{
    startInspectToolBtn->setIcon(QPixmap(":/images/stop.png"));
    startInspectToolBtn->setText(tr("Stop Inspect"));

    setupToolBtn->setEnabled(false);
}

void MainWindow::onInspectStateStopped()
{
    startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
    startInspectToolBtn->setText(tr("Start Inspect"));

    setupToolBtn->setEnabled(true);
}

void MainWindow::onHasImage(const QImage& image)
{
    plot->setImage((QPixmap::fromImage(image)));
}

void MainWindow::onInspectDone(DetectResult result)
{
    QString strText = QString(tr("Current Frame id: %1\n")).arg(result.frameId);
    if (result.result < 0)
        strText += QString(tr("Current Inspect result: Faile\n"));
    else {
        strText += QString(tr("Current Inspect result: Succeed\n"));
        strText += QString(tr("Left Colume Result: %1\n")).arg(result.left_col_result);
        strText += QString(tr("Right Colume Result: %1\n")).arg(result.right_col_result);

        QVector<ZaoInfo> vecZaoInfo = result.curFrameZaoInfo.value<QVector<ZaoInfo> >();
        strText += QString(tr("Product Count: %1\n")).arg(vecZaoInfo.size());
        strText += QString(tr("Product Info:\n"));
        for (int i = 0; i < vecZaoInfo.size(); ++i) {
            strText += QString(tr("  %1. Product Class: %2,"
                                  " Product Positon: (%3, %4, %5, %6)\n")).
                    arg(i + 1).arg(vecZaoInfo[i].classId).
                    arg(vecZaoInfo[i].zaoPos.x).arg(vecZaoInfo[i].zaoPos.y).
                    arg(vecZaoInfo[i].zaoPos.width).arg(vecZaoInfo[i].zaoPos.height);
        }
    }

    resultTB->setText(strText);

    if (mic->getInspectState() == INSPECT_STATE_INSPECTED)
        cic->startNewCache(1);
}

void MainWindow::onSelCamTypeBtnClicked()
{
    QSelectCameraTypeDlg dlg;
    dlg.setPSysInfo(mic->getPSysInfo());
    dlg.exec();
}

void MainWindow::onCommSetupBtnClicked()
{
    QCommSetupDlg dlg;
    dlg.setPSysInfo(mic->getPSysInfo());
    dlg.exec();
}

void MainWindow::onStartSysBtnClicked()
{
    if (mic->getSysState() == SYS_STATE_IDLE) {
        if (mic->startSys() == 0) {
            onSystemStateStarted();
        }
        else
            QMessageBox::information(this, QString(tr("Start System Failed")),
                                     QString(tr("Start system failed."
                                                " Please check whether the"
                                                " camera or the serial com"
                                                " is connect correctly.")));
    }
    else {
        if (mic->stopSys() == 0) {
            onSystemStateStopped();
        }
    }
}

void MainWindow::onStartInspectBtnClicked()
{
    if (mic->getInspectState() == INSPECT_STATE_WAIT) {
        if (mic->startInspect() == 0) {
            onInspectStateStarted();
        }
    }
    else if (mic->getInspectState() == INSPECT_STATE_INSPECTED) {
        if (mic->stopInspect() == 0) {
            onInspectStateStopped();
        }
    }
}

void MainWindow::onFrameCalActionTriggered()
{
    QFrameCalInfoSetupDlg dlg;
    dlg.setPFramCalInfo(mic->getPFrameCalInfo());
    if (dlg.exec() == QDialog::Accepted)
        mic->updateFrameDist();
}

void MainWindow::onAlgParasActionTriggered()
{
    QAlgParasSetupDlg dlg;
    dlg.setPInspectAlgParas(mic->getPInspectAlgParas());
    if (dlg.exec() == QDialog::Accepted)
        mic->resetInspectParas();
}

void MainWindow::onModbusCmdActionTriggered()
{
    QRtuOperatorDlg dlg;
    dlg.setRwCommInst(mic->getRwCommInst());
    dlg.exec();
}

