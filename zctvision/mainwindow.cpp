#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

#include "FileLogger.h"
#include "qselectcameratypedlg.h"
#include "qframecalinfosetupdlg.h"
#include "qalgparassetupdlg.h"

#include <QDateTime>


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
                                               "Frame Calibration Setup", this);
    QAction* algParasSetupAction = new QAction(QPixmap(":/images/parameter.png"),
                                               "Algorithm Parameters Setup", this);
    connect(frameCalSetupAction, &QAction::triggered,
            this, &MainWindow::onFrameCalActionTriggered);
    connect(algParasSetupAction, &QAction::triggered,
            this, &MainWindow::onAlgParasActionTriggered);

    setupMenu->addAction(frameCalSetupAction);
    setupMenu->addAction(algParasSetupAction);

    setupToolBtn->setMenu(setupMenu);
    setupToolBtn->setPopupMode(QToolButton::MenuButtonPopup);

    ui->mainToolBar->addWidget(selCamTypeToolBtn);
    ui->mainToolBar->addWidget(startSysToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(startInspectToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(setupToolBtn);

    qDebug() << "Current currentThreadId" << QThread::currentThreadId();

    plot = new GraphicsWidget(this);

    QLabel* resultLabel = new QLabel("Result:", this);
    resultLE = new QLineEdit(this);

    QVBoxLayout* btnLayout = new QVBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(resultLabel);
    btnLayout->addWidget(resultLE);

    QWidget* mainWidget = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(mainWidget);
    topLayout->addWidget(plot, 1);
    topLayout->addLayout(btnLayout);

    setCentralWidget(mainWidget);

    //connect
    connect(&rc, &MainResource::hasImage,
            this, &MainWindow::onHasImage);

    connect(&rc, &MainResource::inspectDone,
            this, &MainWindow::onInspectDone);

    setWindowState(this->windowState() ^ Qt::WindowMaximized);
    setWindowTitle(tr("Jujube Inspect Program"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onHasImage(const QImage& image)
{
    plot->setImage((QPixmap::fromImage(image)));
}

void MainWindow::onInspectDone(DetectResult result)
{
    resultLE->setText(QString("%1: %2").arg(result.frameId).arg(result.result));

    if (rc.getInspectState() == INSPECT_STATE_INSPECTED)
        cic->startNewCache(1);
}

void MainWindow::onSelCamTypeBtnClicked()
{
    QSelectCameraTypeDlg dlg;
    dlg.setPSysInfo(rc.getPSysInfo());
    dlg.exec();
}

void MainWindow::onStartSysBtnClicked()
{
    if (rc.getSysState() == SYS_STATE_IDLE) {
        if (rc.startSys() == 0) {
            selCamTypeToolBtn->setEnabled(false);
            startSysToolBtn->setIcon(QPixmap(":/images/stop_sys.png"));
            startSysToolBtn->setText(tr("Stop System"));

            startInspectToolBtn->setEnabled(true);
        }
    }
    else {
        if (rc.stopSys() == 0) {
            selCamTypeToolBtn->setEnabled(true);
            startSysToolBtn->setIcon(QPixmap(":/images/start_sys.png"));
            startSysToolBtn->setText(tr("Start System"));

            startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
            startInspectToolBtn->setText(tr("Start Inspect"));
            startInspectToolBtn->setEnabled(false);

            setupToolBtn->setEnabled(true);
        }
    }
}

void MainWindow::onStartInspectBtnClicked()
{
    if (rc.getInspectState() == INSPECT_STATE_WAIT) {
        if (rc.startInspect() == 0) {
            startInspectToolBtn->setIcon(QPixmap(":/images/stop.png"));
            startInspectToolBtn->setText(tr("Stop Inspect"));

            setupToolBtn->setEnabled(false);
        }
    }
    else if (rc.getInspectState() == INSPECT_STATE_INSPECTED) {
        if (rc.stopInspect() == 0) {
            startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
            startInspectToolBtn->setText(tr("Start Inspect"));

            setupToolBtn->setEnabled(true);
        }
    }
}

void MainWindow::onFrameCalActionTriggered()
{
    QFrameCalInfoSetupDlg dlg;
    dlg.setPFramCalInfo(rc.getPFrameCalInfo());
    if (dlg.exec() == QDialog::Accepted)
        rc.initAlg();
}

void MainWindow::onAlgParasActionTriggered()
{
    QAlgParasSetupDlg dlg;
    dlg.setPInspectAlgParas(rc.getPInspectAlgParas());
    dlg.exec();
}

