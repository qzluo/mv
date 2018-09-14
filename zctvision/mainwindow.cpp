#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdebug.h"

#include "FileLogger.h"
#include "qselectcameratypedlg.h"
#include "qcommsetupdlg.h"
#include "qframecalinfosetupdlg.h"
#include "qalgparassetupdlg.h"
#include "qrtuoperatordlg.h"
#include "qinspectparassetupdlg.h"
#include "qgradeparassetupdlg.h"
#include "qsysparassetupdlg.h"

#include <QDate>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    curImage = QImage();    //当前待检测的图片
    curImageFileName = "";
    hasNext = false;
    hasPrev = false;

    ui->setupUi(this);

    setIconSize(QSize(40,40));

    //init UI
    //open cfg file
    openCfgFileToolBtn = new QToolButton(this);
    openCfgFileToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    openCfgFileToolBtn->setIcon(QPixmap(":/images/open_cfg.png"));
    openCfgFileToolBtn->setToolTip(tr("Open Config File"));
    connect(openCfgFileToolBtn, &QToolButton::clicked,
            this, &MainWindow::onOpenCfgFileBtnClicked);

    //save cfg file
    saveCfgFileToolBtn = new QToolButton(this);
    saveCfgFileToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    saveCfgFileToolBtn->setIcon(QPixmap(":/images/save_cfg.png"));
    saveCfgFileToolBtn->setToolTip(tr("Save Config File"));
    connect(saveCfgFileToolBtn, &QToolButton::clicked,
            this, &MainWindow::onSaveCfgFileBtnClicked);

    //load algorithm parameters file
    loadAlgParasFileToolBtn = new QToolButton(this);
    loadAlgParasFileToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    loadAlgParasFileToolBtn->setIcon(QPixmap(":/images/open_paras.png"));
    loadAlgParasFileToolBtn->setToolTip(tr("Load Algorithm Parameters File"));
    connect(loadAlgParasFileToolBtn, &QToolButton::clicked,
            this, &MainWindow::onLoadAlgParasFileBtnClicked);

    //save algorithm parameters
    saveAlgParasToolBtn = new QToolButton(this);
    saveAlgParasToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    saveAlgParasToolBtn->setIcon(QPixmap(":/images/save_paras.png"));
    saveAlgParasToolBtn->setToolTip(tr("Save Algorithm Parameters"));
    connect(saveAlgParasToolBtn, &QToolButton::clicked,
            this, &MainWindow::onSaveAlgParasBtnClicked);

    //save algorithm parameters to file
    saveAlgParasToFileToolBtn = new QToolButton(this);
    saveAlgParasToFileToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    saveAlgParasToFileToolBtn->setIcon(QPixmap(":/images/save_paras_as.png"));
    saveAlgParasToFileToolBtn->setToolTip(tr("Save Algorithm Parameters To File"));
    connect(saveAlgParasToFileToolBtn, &QToolButton::clicked,
            this, &MainWindow::onSaveAlgParasToFileBtnClicked);

    //select camera type
    selCamTypeToolBtn = new QToolButton(this);
    selCamTypeToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    selCamTypeToolBtn->setIcon(QPixmap(":/images/select_camera.png"));
    selCamTypeToolBtn->setToolTip(tr("Select Camera Type"));
    connect(selCamTypeToolBtn, &QToolButton::clicked,
            this, &MainWindow::onSelCamTypeBtnClicked);

    //comm setup
    commSetupToolBtn = new QToolButton(this);
    commSetupToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    commSetupToolBtn->setIcon(QPixmap(":/images/serialcomm.png"));
    commSetupToolBtn->setToolTip(tr("Serial Setup"));
    connect(commSetupToolBtn, &QToolButton::clicked,
            this, &MainWindow::onCommSetupBtnClicked);

    //system paras
    sysParasToolBtn = new QToolButton(this);
    sysParasToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    sysParasToolBtn->setIcon(QPixmap(":/images/sys_paras.png"));
    sysParasToolBtn->setToolTip(tr("System Parameters"));
    connect(sysParasToolBtn, &QToolButton::clicked,
            this, &MainWindow::onSysParasSetupBtnClicked);

    //start system
    startSysToolBtn = new QToolButton(this);
    startSysToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    startSysToolBtn->setIcon(QPixmap(":/images/start_sys.png"));
    startSysToolBtn->setToolTip(tr("Start System"));
    connect(startSysToolBtn, &QToolButton::clicked,
            this, &MainWindow::onStartSysBtnClicked);

    //open image inspect
    openImageToolBtn = new QToolButton(this);
    openImageToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    openImageToolBtn->setIcon(QPixmap(":/images/open_image.png"));
    openImageToolBtn->setToolTip(tr("Open Image"));
    connect(openImageToolBtn, &QToolButton::clicked,
            this, &MainWindow::onOpenImageBtnClicked);

    //next image inspect
    nextImageToolBtn = new QToolButton(this);
    nextImageToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    nextImageToolBtn->setIcon(QPixmap(":/images/next_image.png"));
    nextImageToolBtn->setToolTip(tr("Next Image"));
    connect(nextImageToolBtn, &QToolButton::clicked,
            this, &MainWindow::onNextImageBtnClicked);

    //last image inspect
    lastImageToolBtn = new QToolButton(this);
    lastImageToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    lastImageToolBtn->setIcon(QPixmap(":/images/prev_image.png"));
    lastImageToolBtn->setToolTip(tr("Last Image"));
    connect(lastImageToolBtn, &QToolButton::clicked,
            this, &MainWindow::onLastImageBtnClicked);

    //control board paras
    ctlboardParasToolBtn = new QToolButton(this);
    ctlboardParasToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ctlboardParasToolBtn->setIcon(QPixmap(":/images/modbus_cmd.png"));
    ctlboardParasToolBtn->setToolTip(tr("Control Board Parameters"));
    connect(ctlboardParasToolBtn, &QToolButton::clicked,
            this, &MainWindow::onModbusCmdActionTriggered);

    //statics info
    staticsInfosToolBtn = new QToolButton(this);
    staticsInfosToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    staticsInfosToolBtn->setIcon(QPixmap(":/images/stastics.png"));
    staticsInfosToolBtn->setToolTip(tr("Stastics Infomation"));
//    connect(ctlboardParasToolBtn, &QToolButton::clicked,
//            this, &MainWindow::onLastImageBtnClicked);

    //setup
//    setupToolBtn = new QToolButton(this);
//    setupToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
//    setupToolBtn->setIcon(QPixmap(":/images/tool_icon.png"));
//    setupToolBtn->setToolTip(tr("Setup"));

//    QMenu* setupMenu = new QMenu(this);
//    QAction* frameCalSetupAction = new QAction(QPixmap(":/images/calibration.png"),
//                                               tr("Frame Calibration Setup"), this);
//    QAction* algParasSetupAction = new QAction(QPixmap(":/images/parameter.png"),
//                                               tr("Algorithm Parameters Setup"), this);
//    modbusCmdAction = new QAction(QPixmap(":/images/modbus_cmd.png"),
//                                               tr("Modbus Command"), this);
//    connect(frameCalSetupAction, &QAction::triggered,
//            this, &MainWindow::onFrameCalActionTriggered);
//    connect(algParasSetupAction, &QAction::triggered,
//            this, &MainWindow::onAlgParasActionTriggered);
//    connect(modbusCmdAction, &QAction::triggered,
//            this, &MainWindow::onModbusCmdActionTriggered);

//    setupMenu->addAction(frameCalSetupAction);
//    setupMenu->addAction(algParasSetupAction);
//    setupMenu->addAction(modbusCmdAction);

//    setupToolBtn->setMenu(setupMenu);
//    setupToolBtn->setPopupMode(QToolButton::MenuButtonPopup);

    ui->mainToolBar->addWidget(openCfgFileToolBtn);
    ui->mainToolBar->addWidget(saveCfgFileToolBtn);
    ui->mainToolBar->addWidget(selCamTypeToolBtn);
    ui->mainToolBar->addWidget(commSetupToolBtn);
    ui->mainToolBar->addWidget(sysParasToolBtn);
    ui->mainToolBar->addWidget(startSysToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addWidget(loadAlgParasFileToolBtn);
    ui->mainToolBar->addWidget(saveAlgParasToolBtn);
    ui->mainToolBar->addWidget(saveAlgParasToFileToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addWidget(openImageToolBtn);
    ui->mainToolBar->addWidget(lastImageToolBtn);
    ui->mainToolBar->addWidget(nextImageToolBtn);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addSeparator();

    ui->mainToolBar->addWidget(ctlboardParasToolBtn);
    ui->mainToolBar->addWidget(staticsInfosToolBtn);
//    ui->mainToolBar->addSeparator();
//    ui->mainToolBar->addSeparator();

//    ui->mainToolBar->addWidget(setupToolBtn);


    plot = new GraphicsWidget(this);

    //operate btn
    //start inspect
    startInspectToolBtn = new QToolButton(this);
    startInspectToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
    startInspectToolBtn->setFixedSize(150, 50);
    startInspectToolBtn->setIconSize(QSize(120, 40));
    startInspectToolBtn->setToolTip(tr("Start Inspect"));
    startInspectToolBtn->setEnabled(false);
    connect(startInspectToolBtn, &QToolButton::clicked,
            this, &MainWindow::onStartInspectBtnClicked);

    //run once btn
    runOnceBtn = new QToolButton(this);
    runOnceBtn->setText(tr("Run Once"));
    runOnceBtn->setFixedSize(150, 50);
    connect(runOnceBtn, &QToolButton::clicked,
            this, &MainWindow::onRunOnceBtnClicked);

    QHBoxLayout* opLayout = new QHBoxLayout;
    opLayout->addWidget(startInspectToolBtn);
    opLayout->addWidget(runOnceBtn);

    //result
    QLabel* resultLabel = new QLabel(tr("Inspect Output Result:"), this);
    resultTB = new QTextBrowser(this);

    QGroupBox* realResultGB = new QGroupBox(tr("Real Time Result"), this);
    QHBoxLayout* realResultLayout = new QHBoxLayout(realResultGB);

    rtResultWidget = new QRealTimeResultWidget(this);
    realResultLayout->addWidget(rtResultWidget);

    //setup paras
    QGroupBox* setupGB = new QGroupBox(tr("Inspect Parameter Setup"), this);
    QVBoxLayout* setupLayout = new QVBoxLayout(setupGB);
    recognizeParasBtn = new QPushButton(tr("Recognize Parameters"), this);
    gradeParasBtn = new QPushButton(tr("Grade Parameters"), this);

    connect(recognizeParasBtn, &QPushButton::clicked,
            this, &MainWindow::onRecognizeParasBtnClicked);
    connect(gradeParasBtn, &QPushButton::clicked,
            this, &MainWindow::onGradeParasBtnClicked);

    setupLayout->addWidget(recognizeParasBtn);
    setupLayout->addWidget(gradeParasBtn);
    setupLayout->addStretch();

    QVBoxLayout* resultLayout = new QVBoxLayout;
    resultLayout->addLayout(opLayout);
    resultLayout->addWidget(realResultGB);
    resultLayout->addWidget(resultLabel);
    resultLayout->addWidget(resultTB);
    resultLayout->addWidget(setupGB);

    resultLayout->addStretch();

    QWidget* mainWidget = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(mainWidget);
    topLayout->addWidget(plot, 10);
    topLayout->addLayout(resultLayout,1);

    setCentralWidget(mainWidget);

    QRect screenRect = QApplication::desktop()->screenGeometry();
    resize(screenRect.width()-100,screenRect.height()-100);
    setWindowState(this->windowState() ^ Qt::WindowMaximized);

    setWindowTitle(tr("Jujube Inspect Program"));

    //状态栏
    statusInfo = new QLabel(this);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 设置不显示label的边框
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    statusBar()->addWidget(statusInfo);
    showStatus();

    //init btn state
    startInspectToolBtn->setEnabled(false);

    openCfgFileToolBtn->setEnabled(true);
    saveCfgFileToolBtn->setEnabled(true);

    loadAlgParasFileToolBtn->setEnabled(true);
    saveAlgParasToolBtn->setEnabled(true);
    saveAlgParasToFileToolBtn->setEnabled(true);

    selCamTypeToolBtn->setEnabled(true);
    commSetupToolBtn->setEnabled(true);
    sysParasToolBtn->setEnabled(true);

    openImageToolBtn->setEnabled(true);
    nextImageToolBtn->setEnabled(false);
    lastImageToolBtn->setEnabled(false);

    ctlboardParasToolBtn->setEnabled(false);

    recognizeParasBtn->setEnabled(true);
    gradeParasBtn->setEnabled(true);
    runOnceBtn->setEnabled(false);

    qDebug() << "Current currentThreadId" << QThread::currentThreadId();

//    tester.test();

//    tester.testInit();
//    tester.testInspectSigleImage();

//    testHasNextAndLastFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    mic->init();
}

void MainWindow::showStatus()
{
    QString sysStateDesc = mic->getSysState() == SYS_STATE_IDLE ?
                QString(tr("Stopped")) : QString(tr("Started"));

    QString inspectStateDesc = mic->getInspectState() == INSPECT_STATE_WAIT ?
                QString(tr("Unstarted")) : QString(tr("Started"));

    QString msg = QString(tr("System state: %1.\t    "
                             "Inspect state: %2."))
            .arg(sysStateDesc).arg(inspectStateDesc);

    statusInfo->setText(msg);
}

void MainWindow::onSystemStateStarted()
{
    selCamTypeToolBtn->setEnabled(false);
    commSetupToolBtn->setEnabled(false);

    startSysToolBtn->setIcon(QPixmap(":/images/stop_sys.png"));
    startSysToolBtn->setToolTip(tr("Stop System"));

    startInspectToolBtn->setEnabled(true);

    //相机及串口资源已初始化，不能更改
    openCfgFileToolBtn->setEnabled(false);
    selCamTypeToolBtn->setEnabled(false);
    commSetupToolBtn->setEnabled(false);

    ctlboardParasToolBtn->setEnabled(true);
}

void MainWindow::onSystemStateStopped()
{
    selCamTypeToolBtn->setEnabled(true);
    commSetupToolBtn->setEnabled(true);
    startSysToolBtn->setIcon(QPixmap(":/images/start_sys.png"));
    startSysToolBtn->setToolTip(tr("Start System"));

    startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
    startInspectToolBtn->setToolTip(tr("Start Inspect"));
    startInspectToolBtn->setEnabled(false);

    openCfgFileToolBtn->setEnabled(true);
    saveCfgFileToolBtn->setEnabled(true);

    loadAlgParasFileToolBtn->setEnabled(true);
    saveAlgParasToolBtn->setEnabled(true);
    saveAlgParasToFileToolBtn->setEnabled(true);

    selCamTypeToolBtn->setEnabled(true);
    commSetupToolBtn->setEnabled(true);
    sysParasToolBtn->setEnabled(true);

    openImageToolBtn->setEnabled(true);

    recognizeParasBtn->setEnabled(true);
    gradeParasBtn->setEnabled(true);

    ctlboardParasToolBtn->setEnabled(false);
//    setupToolBtn->setEnabled(true);
}

void MainWindow::onInspectStateStarted()
{
    startInspectToolBtn->setIcon(QPixmap(":/images/stop.png"));
    startInspectToolBtn->setToolTip(tr("Stop Inspect"));

    //connect
    connect(mic, &MainResource::hasImage,
            this, &MainWindow::onHasImage);

    connect(mic, &MainResource::inspectDone,
            this, &MainWindow::onInspectDone);

    curImageFileName = "";
    hasNext = false;
    hasPrev = false;
    nextImageToolBtn->setEnabled(false);
    lastImageToolBtn->setEnabled(false);

    saveCfgFileToolBtn->setEnabled(false);

    sysParasToolBtn->setEnabled(false);
    ctlboardParasToolBtn->setEnabled(false);

    loadAlgParasFileToolBtn->setEnabled(false);
    saveAlgParasToolBtn->setEnabled(false);
    saveAlgParasToFileToolBtn->setEnabled(false);
    openImageToolBtn->setEnabled(false);

    runOnceBtn->setEnabled(false);
    recognizeParasBtn->setEnabled(false);
    gradeParasBtn->setEnabled(false);

//    setupToolBtn->setEnabled(false);
}

void MainWindow::onInspectStateStopped()
{
    startInspectToolBtn->setIcon(QPixmap(":/images/start.png"));
    startInspectToolBtn->setToolTip(tr("Start Inspect"));

    //disconnect
    disconnect(mic, &MainResource::hasImage,
               this, &MainWindow::onHasImage);

    disconnect(mic, &MainResource::inspectDone,
               this, &MainWindow::onInspectDone);

    saveCfgFileToolBtn->setEnabled(true);

    loadAlgParasFileToolBtn->setEnabled(true);
    saveAlgParasToolBtn->setEnabled(true);
    saveAlgParasToFileToolBtn->setEnabled(true);
    sysParasToolBtn->setEnabled(true);
    openImageToolBtn->setEnabled(true);

    ctlboardParasToolBtn->setEnabled(true);

    recognizeParasBtn->setEnabled(true);
    gradeParasBtn->setEnabled(true);

//    setupToolBtn->setEnabled(true);
}

void MainWindow::hasNextAndLastFile(QString &fullFileName, bool *pHasNext, bool *bHasLast)
{
    if (fullFileName.isEmpty()) {
        *pHasNext = false;
        *bHasLast = false;
        return;
    }

    int nameIndex = fullFileName.lastIndexOf("\\");
    if (nameIndex < 0)
        nameIndex = fullFileName.lastIndexOf("/");

    if (nameIndex < 0) {
        *pHasNext = false;
        *bHasLast = false;
        return;
    }

    //get file name
    QString fileName = fullFileName.right(fullFileName.length() - nameIndex - 1);
    QStringList fileNameSplits = fileName.split(".");
    if (fileNameSplits.length() <= 0) {
        *pHasNext = false;
        *bHasLast = false;
        return;
    }

    bool bOk = false;

    //file name to number
    int fileNameNum = fileNameSplits[0].toInt(&bOk);
    if (!bOk) {
        *pHasNext = false;
        *bHasLast = false;
        return;
    }

    QString nextFileFullName = fullFileName.left(nameIndex + 1) +
            fileName.replace(QString::number(fileNameNum),
                             QString::number(fileNameNum+1));

    *pHasNext = QFile::exists(nextFileFullName);

    QString lastFileFullName = fullFileName.left(nameIndex + 1) +
            fileName.replace(QString::number(fileNameNum+1),
                             QString::number(fileNameNum-1));

    *bHasLast = QFile::exists(lastFileFullName);
}

bool MainWindow::hasNextFile(QString &fullFileName)
{
    if (fullFileName.isEmpty())
        return false;

    int nameIndex = fullFileName.lastIndexOf("\\");
    if (nameIndex < 0)
        nameIndex = fullFileName.lastIndexOf("/");

    if (nameIndex < 0)
        return false;

    //get file name
    QString fileName = fullFileName.right(fullFileName.length() - nameIndex - 1);
    QStringList fileNameSplits = fileName.split(".");
    if (fileNameSplits.length() <= 0)
        return false;

    bool bOk = false;

    //file name to number
    int fileNameNum = fileNameSplits[0].toInt(&bOk);
    if (!bOk)
        return false;

    QString nextFileFullName = fullFileName.left(nameIndex + 1) +
            fileName.replace(fileNameSplits[0], QString::number(fileNameNum+1));

    QFile file(nextFileFullName);
    return file.exists();
}

QString MainWindow::getNextOrLastFileName(QString& fullFileName, bool bNext)
{
    if (fullFileName.isEmpty())
        return "";

    int nameIndex = fullFileName.lastIndexOf("\\");
    if (nameIndex < 0)
        nameIndex = fullFileName.lastIndexOf("/");

    if (nameIndex < 0)
        return "";

    //get file name
    QString fileName = fullFileName.right(fullFileName.length() - nameIndex - 1);
    QStringList fileNameSplits = fileName.split(".");
    if (fileNameSplits.length() <= 0)
        return "";

    bool bOk = false;

    //file name to number
    int fileNameNum = fileNameSplits[0].toInt(&bOk);
    if (!bOk)
        return "";

    int newFileNameNum = bNext ? (fileNameNum + 1) : (fileNameNum - 1);
    QString retFileFullName = fullFileName.left(nameIndex + 1) +
            fileName.replace(QString::number(fileNameNum),
                             QString::number(newFileNameNum));

    return QFile::exists(retFileFullName) ? retFileFullName : "";
}

void MainWindow::onNextOrLastImageBtnClicked(bool bNext)
{
    if (curImageFileName.isEmpty())
        return;

    if (mic->getInspectState() == INSPECT_STATE_INSPECTED)
        return;

    QString fileName = getNextOrLastFileName(curImageFileName, bNext);

    if (fileName.isEmpty())
        return;

    QImage newImage = QImage(fileName);
    if (newImage.isNull())
        return;

    //update curImage
    curImage = newImage;
    curImageFileName = fileName;
    hasNextAndLastFile(curImageFileName, &hasNext, &hasPrev);
    nextImageToolBtn->setEnabled(hasNext);
    lastImageToolBtn->setEnabled(hasPrev);

    plot->setImage(QPixmap::fromImage(curImage));

    onRunOnceBtnClicked();
}

void MainWindow::testHasNextFile()
{
    qDebug() << hasNextFile(QString(""));
    qDebug() << hasNextFile(QString("abc"));
    qDebug() << hasNextFile(QString("F:\\VirCam\\TmpImage1\\8058.bmp"));
}

void MainWindow::testHasNextAndLastFile()
{
    bool tmpHasNext = false;
    bool tmpHasLast = false;

    hasNextAndLastFile(QString(""), &tmpHasNext, &tmpHasLast);
    qDebug() << tmpHasNext << tmpHasLast;

    hasNextAndLastFile(QString("abc"), &tmpHasNext, &tmpHasLast);
    qDebug() << tmpHasNext << tmpHasLast;

    hasNextAndLastFile(QString("F:\\VirCam\\TmpImage1\\8058.bmp"), &tmpHasNext, &tmpHasLast);
    qDebug() << tmpHasNext << tmpHasLast;

    hasNextAndLastFile(QString("F:\\VirCam\\TmpImage1\\8059.bmp"), &tmpHasNext, &tmpHasLast);
    qDebug() << tmpHasNext << tmpHasLast;

    hasNextAndLastFile(QString("F:\\VirCam\\TmpImage1\\8287.bmp"), &tmpHasNext, &tmpHasLast);
    qDebug() << tmpHasNext << tmpHasLast;


}

void MainWindow::onHasImage(const QImage& image)
{
    curImage = image.copy();
    plot->setImage((QPixmap::fromImage(image)));

    if (mic->getPSysInfo()->getSaveImageIsOpened()) {
        //将文件保存到指定目录
        QString curDate = QDate::currentDate().toString("yy.MM.dd");
        QString filePath = QString("%1\\images\\%2\\").
                arg(mic->getPSysInfo()->getTmpFilePath()).arg(curDate);

        QDir dir;
        if (!dir.exists(filePath)) {
            if (!dir.mkpath(filePath)) {
                qDebug() << "failed to create dir";
                return;
            }
        }

        int curFrameId = cic->getCurFrameId();
        QString fileName = filePath + QString("%1.bmp").arg(curFrameId);

        if (curImage.save(fileName, "BMP")) {
            qDebug() << "save image successful";
        }
        else
            qDebug() << "failed to save image";
    }
}

void MainWindow::onInspectDone(DetectResult result)
{
    QString strText = QString(tr("Current Frame id: %1\n")).arg(result.frameId);
    if (result.result < 0)
        strText += QString(tr("Current Inspect result: Faile\n"));
    else {
        strText += QString(tr("Current Inspect result: Succeed\n"));
        strText += QString(tr("Left Colume Grade Result: %1\n")).arg(result.left_col_grade_result);
        strText += QString(tr("Right Colume Grade Result: %1\n")).arg(result.right_col_grade_result);

        QList<ZaoInfo> vecZaoInfo = result.curFrameZaoInfo.value<QList<ZaoInfo> >();
        //show in the interface
//        strText += QString(tr("Product Count: %1\n")).arg(vecZaoInfo.size());
//        strText += QString(tr("Product Info:\n"));
//        for (int i = 0; i < vecZaoInfo.size(); ++i) {
//            strText += QString(tr("  %1. Product Class: %2,"
//                                  " Product Positon: (%3, %4, %5, %6)\n")).
//                    arg(i + 1).arg(vecZaoInfo[i].classId).
//                    arg(vecZaoInfo[i].zaoPos.x).arg(vecZaoInfo[i].zaoPos.y).
//                    arg(vecZaoInfo[i].zaoPos.width).arg(vecZaoInfo[i].zaoPos.height);
//        }

        Q_ASSERT(vecZaoInfo.size() == ZAO_REGION_COUNT * 2);

        for (int i = 0; i < vecZaoInfo.size(); ++i) {
            if (vecZaoInfo.at(i).classId == ZAO_CLASS_NONE)
                rtResultWidget->setItemResult(i, tr("NULL"), QString(""), QString(""));
            else
                rtResultWidget->setItemResult(i, getJujubeDescFromClass(vecZaoInfo.at(i).classId),
                                              QString::number(vecZaoInfo.at(i).zaoPos.width),
                                              QString::number(vecZaoInfo.at(i).zaoPos.height));
        }
    }

    resultTB->setText(strText);

    if (mic->getInspectState() == INSPECT_STATE_INSPECTED)
        cic->startNewInspect();
}

void MainWindow::onOpenCfgFileBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.cfg)"));

    if (fileName.isEmpty())
        return;

    if (mic->loadCfgFile(fileName) < 0) {
        QMessageBox::information(this, QString(tr("Load Config File Failed")),
                                 QString(tr("Load config file failed."
                                            " Please check whether the"
                                            " file is a valid config file.")));
    }
}

void MainWindow::onSaveCfgFileBtnClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "",
                                                     tr("Files (*.cfg)"));

    if (fileName.isEmpty())
        return;

    if (mic->saveCfgFile(fileName) < 0) {
        QMessageBox::information(this, QString(tr("Save Config File Failed")),
                                 QString(tr("Failed to save file.")));
    }
}

void MainWindow::onLoadAlgParasFileBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.xml)"));

    if (fileName.isEmpty())
        return;

    if (mic->reloadAlgParasFile(fileName) < 0) {
        QMessageBox::information(this, QString(tr("Load File Failed")),
                                 QString(tr("Load algorithm parameters file failed."
                                            " Please check whether the"
                                            " file is a valid algorithm parameters file.")));
    }
}

void MainWindow::onSaveAlgParasBtnClicked()
{
    mic->saveAlgParas();
}

void MainWindow::onSaveAlgParasToFileBtnClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "",
                                                     tr("Files (*.xml)"));

    if (fileName.isEmpty())
        return;

    if (mic->saveAlgParasToFile(fileName) < 0) {
        QMessageBox::information(this, QString(tr("Save Parameters File Failed")),
                                 QString(tr("Failed to save file.")));
    }
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

void MainWindow::onSysParasSetupBtnClicked()
{
    QSysParasSetupDlg dlg;
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

    showStatus();
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

    showStatus();
}

void MainWindow::onOpenImageBtnClicked()
{
    if (mic->getInspectState() == INSPECT_STATE_INSPECTED)
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Images (*.png *.bmp *.jpg)"));

    if (fileName.isEmpty())
        return;

    QImage newImage = QImage(fileName);
    if (newImage.isNull())
        return;

    //update curImage
    curImage = newImage;
    curImageFileName = fileName;
    hasNextAndLastFile(curImageFileName, &hasNext, &hasPrev);
    nextImageToolBtn->setEnabled(hasNext);
    lastImageToolBtn->setEnabled(hasPrev);
    runOnceBtn->setEnabled(true);

    plot->setImage(QPixmap::fromImage(curImage));

    onRunOnceBtnClicked();
}

void MainWindow::onNextImageBtnClicked()
{
    onNextOrLastImageBtnClicked(true);
}

void MainWindow::onLastImageBtnClicked()
{
    onNextOrLastImageBtnClicked(false);
}

void MainWindow::onRunOnceBtnClicked()
{
    if (curImage.isNull())
        return ;

    QList<ZaoInfo> cur_left_col_result;
    QList<ZaoInfo> cur_right_col_result;
    if (mic->inspectSingleImage(curImage, cur_left_col_result, cur_right_col_result) < 0)
        return ;

    //show result
    Q_ASSERT(cur_left_col_result.size() == ZAO_REGION_COUNT);
    Q_ASSERT(cur_right_col_result.size() == ZAO_REGION_COUNT);

    for (int i = 0; i < ZAO_REGION_COUNT; ++i) {
        if (cur_left_col_result.at(i).classId == ZAO_CLASS_NONE) {
            rtResultWidget->setItemResult(i, tr("NULL"), QString(""), QString(""));
        }
        else
            rtResultWidget->setItemResult(i, getJujubeDescFromClass(cur_left_col_result.at(i).classId),
                                          QString::number(cur_left_col_result.at(i).zaoPos.width),
                                          QString::number(cur_left_col_result.at(i).zaoPos.height));

        if (cur_right_col_result.at(i).classId == ZAO_CLASS_NONE) {
            rtResultWidget->setItemResult(i+4, tr("NULL"), QString(""), QString(""));
        }
        else
            rtResultWidget->setItemResult(i+4, getJujubeDescFromClass(cur_right_col_result.at(i).classId),
                                          QString::number(cur_right_col_result.at(i).zaoPos.width),
                                          QString::number(cur_right_col_result.at(i).zaoPos.height));
    }
}

void MainWindow::onRecognizeParasBtnClicked()
{
    QInspectParasSetupDlg dlg(this);
    dlg.setPInspectAlgParas(mic->getPInspectAlgParas());
    if (dlg.exec() == QDialog::Accepted)
        mic->resetInspectParas();
}

void MainWindow::onGradeParasBtnClicked()
{
    QGradeParasSetupDlg dlg(this);
    dlg.setPInspectAlgParas(mic->getPInspectAlgParas());
    dlg.exec();
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

