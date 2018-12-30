#include "qalgparassetupdlg.h"
#include "Mat2QImage.h"
#include "qsysdefine.h"

QAlgParasSetupDlg::QAlgParasSetupDlg(QWidget *parent) : QDialog(parent)
{
    pInspectAlgParas = NULL;
    image = QImage();

    zaoInspectAlg.init();

    //select image
    QPushButton* imgSource = new QPushButton(tr("Select Image"), this);
    connect(imgSource, &QPushButton::clicked,
            this, &QAlgParasSetupDlg::onSelImgBtnClicked);

    //Good Jujube Parameters
    //1
    QLabel* class1LengthLbl = new QLabel(tr("Class1 Length: "));
    class1LengthDSB = new QDoubleSpinBox(this);
    class1LengthDSB->setRange(0, 10000);
    QLabel* class1LengthUnit = new QLabel("mm");

    QLabel* class1WidthLbl = new QLabel(tr("Class1 Width: "));
    class1WidthDSB = new QDoubleSpinBox(this);
    class1WidthDSB->setRange(0, 10000);
    QLabel* class1WidthUnit = new QLabel("mm");

    QHBoxLayout* class1Layout = new QHBoxLayout();
    class1Layout->addWidget(class1LengthLbl);
    class1Layout->addWidget(class1LengthDSB);
    class1Layout->addWidget(class1LengthUnit);
    class1Layout->addSpacing(10);
    class1Layout->addWidget(class1WidthLbl);
    class1Layout->addWidget(class1WidthDSB);
    class1Layout->addWidget(class1WidthUnit);

    //2
    QLabel* class2LengthLbl = new QLabel(tr("Class2 Length: "));
    class2LengthDSB = new QDoubleSpinBox(this);
    class2LengthDSB->setRange(0, 10000);
    QLabel* class2LengthUnit = new QLabel("mm");

    QLabel* class2WidthLbl = new QLabel(tr("Class2 Width: "));
    class2WidthDSB = new QDoubleSpinBox(this);
    class2WidthDSB->setRange(0, 10000);
    QLabel* class2WidthUnit = new QLabel("mm");

    QHBoxLayout* class2Layout = new QHBoxLayout();
    class2Layout->addWidget(class2LengthLbl);
    class2Layout->addWidget(class2LengthDSB);
    class2Layout->addWidget(class2LengthUnit);
    class2Layout->addSpacing(10);
    class2Layout->addWidget(class2WidthLbl);
    class2Layout->addWidget(class2WidthDSB);
    class2Layout->addWidget(class2WidthUnit);

    //3
    QLabel* class3LengthLbl = new QLabel(tr("Class3 Length: "));
    class3LengthDSB = new QDoubleSpinBox(this);
    class3LengthDSB->setRange(0, 10000);
    QLabel* class3LengthUnit = new QLabel("mm");

    QLabel* class3WidthLbl = new QLabel(tr("Class3 Width: "));
    class3WidthDSB = new QDoubleSpinBox(this);
    class3WidthDSB->setRange(0, 10000);
    QLabel* class3WidthUnit = new QLabel("mm");

    QHBoxLayout* class3Layout = new QHBoxLayout();
    class3Layout->addWidget(class3LengthLbl);
    class3Layout->addWidget(class3LengthDSB);
    class3Layout->addWidget(class3LengthUnit);
    class3Layout->addSpacing(10);
    class3Layout->addWidget(class3WidthLbl);
    class3Layout->addWidget(class3WidthDSB);
    class3Layout->addWidget(class3WidthUnit);

    QGroupBox* goodJujubeParasGB = new QGroupBox(tr("Good Jujube Parameters"), this);
    QVBoxLayout* goodJujubeParasLayout = new QVBoxLayout;
    goodJujubeParasLayout->addLayout(class1Layout);
    goodJujubeParasLayout->addLayout(class2Layout);
    goodJujubeParasLayout->addLayout(class3Layout);
    goodJujubeParasGB->setLayout(goodJujubeParasLayout);

    //detect parameter
    //I
    QLabel* iMinLbl = new QLabel(tr("Imin: "));
    iMinSB = new QSpinBox(this);
    iMinSB->setRange(0, 255);

    QLabel* iMaxLbl = new QLabel(tr("Imax: "));
    iMaxSB = new QSpinBox(this);
    iMaxSB->setRange(0, 255);

    QHBoxLayout* iLayout = new QHBoxLayout();
    iLayout->addWidget(iMinLbl);
    iLayout->addWidget(iMinSB);
    iLayout->addSpacing(20);
    iLayout->addWidget(iMaxLbl);
    iLayout->addWidget(iMaxSB);
    iLayout->addStretch();

    //B
    QLabel* bMinLbl = new QLabel(tr("Bmin: "));
    bMinSB = new QSpinBox(this);
    bMinSB->setRange(0, 255);

    QLabel* bMaxLbl = new QLabel(tr("Bmax: "));
    bMaxSB = new QSpinBox(this);
    bMaxSB->setRange(0, 255);

    QHBoxLayout* bLayout = new QHBoxLayout();
    bLayout->addWidget(bMinLbl);
    bLayout->addWidget(bMinSB);
    bLayout->addSpacing(20);
    bLayout->addWidget(bMaxLbl);
    bLayout->addWidget(bMaxSB);
    bLayout->addStretch();

    //R
    QLabel* rMinLbl = new QLabel(tr("Rmin: "));
    rMinSB = new QSpinBox(this);
    rMinSB->setRange(0, 255);

    QLabel* rMaxLbl = new QLabel(tr("Rmax: "));
    rMaxSB = new QSpinBox(this);
    rMaxSB->setRange(0, 255);

    QHBoxLayout* rLayout = new QHBoxLayout();
    rLayout->addWidget(rMinLbl);
    rLayout->addWidget(rMinSB);
    rLayout->addSpacing(20);
    rLayout->addWidget(rMaxLbl);
    rLayout->addWidget(rMaxSB);
    rLayout->addStretch();

    //w,h
    QLabel* wMinLbl = new QLabel(tr("Wmin: "));
    wMinSB = new QSpinBox(this);
    wMinSB->setRange(0, 1000);

    QLabel* hMinLbl = new QLabel(tr("Hmin: "));
    hMinSB = new QSpinBox(this);
    hMinSB->setRange(0, 1000);

    QLabel* ratioMaxLbl = new QLabel(tr("Max Ratio: "));
    ratioMaxSB = new QDoubleSpinBox(this);
    ratioMaxSB->setRange(0, 1000);

    QHBoxLayout* whLayout = new QHBoxLayout();
    whLayout->addWidget(wMinLbl);
    whLayout->addWidget(wMinSB);
    whLayout->addSpacing(20);
    whLayout->addWidget(hMinLbl);
    whLayout->addWidget(hMinSB);
    whLayout->addSpacing(20);
    whLayout->addWidget(ratioMaxLbl);
    whLayout->addWidget(ratioMaxSB);
    whLayout->addStretch();

    QGroupBox* detectParasGB = new QGroupBox(tr("Detect Parameters"), this);
    QVBoxLayout* detectParasLayout = new QVBoxLayout;
    detectParasLayout->addLayout(iLayout);
    detectParasLayout->addLayout(bLayout);
    detectParasLayout->addLayout(rLayout);
    detectParasLayout->addLayout(whLayout);
    detectParasGB->setLayout(detectParasLayout);

    //test
    QPushButton* testBtn = new QPushButton(tr("Test"), this);
    connect(testBtn, &QPushButton::clicked,
            this, &QAlgParasSetupDlg::onTestBtnClicked);
    QHBoxLayout* testBtnLayout = new QHBoxLayout;
    testBtnLayout->addStretch();
    testBtnLayout->addWidget(testBtn);

    //param
    QFrame* paramFrame = new QFrame(this);
    paramFrame->setFrameShape(QFrame::Box);
    paramFrame->setFrameShadow(QFrame::Raised);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(paramFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QVBoxLayout* parasSetupLayout = new QVBoxLayout(paramFrame);
    parasSetupLayout->addWidget(imgSource);
    parasSetupLayout->addWidget(goodJujubeParasGB);
    parasSetupLayout->addWidget(detectParasGB);
    parasSetupLayout->addStretch();
    parasSetupLayout->addLayout(testBtnLayout);

    plot = new GraphicsWidget(this);
    QGridLayout* mainWindowLayout = new QGridLayout;
    mainWindowLayout->addWidget(plot, 0, 0, 1, 5);
    mainWindowLayout->addWidget(scrollArea, 0, 5, 1, 1);

    QPushButton* okBtn = new QPushButton(tr("Ok"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QAlgParasSetupDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QAlgParasSetupDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->addLayout(mainWindowLayout);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Algrithm Parameters Setup"));

    QRect screenRect = QApplication::desktop()->screenGeometry();
    resize(screenRect.width()-100,screenRect.height()-100);
    setWindowState(Qt::WindowActive/* Qt::WindowMaximized*/);
}

void QAlgParasSetupDlg::onOkBtnClicked()
{
    if (!pInspectAlgParas) {
        accept();
        return;
    }

    updateInspectParas(pInspectAlgParas);

    pInspectAlgParas->save();

    accept();
}

QZaoInspectAlgParas *QAlgParasSetupDlg::getPInspectAlgParas() const
{
    return pInspectAlgParas;
}

void QAlgParasSetupDlg::setPInspectAlgParas(QZaoInspectAlgParas *value)
{
    if (!value)
        return;

    pInspectAlgParas = value;

    class1LengthDSB->setValue(pInspectAlgParas->getClass_good1_length());
    class1WidthDSB->setValue(pInspectAlgParas->getClass_good1_width());
    class2LengthDSB->setValue(pInspectAlgParas->getClass_good2_length());
    class2WidthDSB->setValue(pInspectAlgParas->getClass_good2_width());
    class3LengthDSB->setValue(pInspectAlgParas->getClass_good3_length());
    class3WidthDSB->setValue(pInspectAlgParas->getClass_good3_width());

    iMinSB->setValue(pInspectAlgParas->getImin());
    iMaxSB->setValue(pInspectAlgParas->getImax());
    bMinSB->setValue(pInspectAlgParas->getBmin());
    bMaxSB->setValue(pInspectAlgParas->getBmax());
    rMinSB->setValue(pInspectAlgParas->getRmin());
    rMaxSB->setValue(pInspectAlgParas->getRmax());

    wMinSB->setValue(pInspectAlgParas->getWmin());
    hMinSB->setValue(pInspectAlgParas->getHmin());
    ratioMaxSB->setValue(pInspectAlgParas->getRatioMax());
}

void QAlgParasSetupDlg::updateInspectParas(QZaoInspectAlgParas *pInspectAlgParas)
{
    if (!pInspectAlgParas)
        return;

    pInspectAlgParas->setClass_good1_length(class1LengthDSB->value());
    pInspectAlgParas->setClass_good1_width(class1WidthDSB->value());
    pInspectAlgParas->setClass_good2_length(class2LengthDSB->value());
    pInspectAlgParas->setClass_good2_width(class2WidthDSB->value());
    pInspectAlgParas->setClass_good3_length(class3LengthDSB->value());
    pInspectAlgParas->setClass_good3_width(class3WidthDSB->value());

    pInspectAlgParas->setImin(iMinSB->value());
    pInspectAlgParas->setImax(iMaxSB->value());
    pInspectAlgParas->setBmin(bMinSB->value());
    pInspectAlgParas->setBmax(bMaxSB->value());
    pInspectAlgParas->setRmin(rMinSB->value());
    pInspectAlgParas->setRmax(rMaxSB->value());

    pInspectAlgParas->setWmin(wMinSB->value());
    pInspectAlgParas->setHmin(hMinSB->value());
    pInspectAlgParas->setRatioMax(ratioMaxSB->value());
}

QString QAlgParasSetupDlg::getIdDesc(int classId)
{
    QString strResult = "";

    switch (classId) {
    case ZAO_CLASS_GOOD1:
        strResult = QString::fromUtf8("特等");
        break;

    case ZAO_CLASS_GOOD2:
        strResult = QString::fromUtf8("一等 ");
        break;

    case ZAO_CLASS_GOOD3:
        strResult = QString::fromUtf8("二等");
        break;

    case ZAO_CLASS_GOOD4:
        strResult = QString::fromUtf8("三等");
        break;

    case ZAO_CLASS_BAD1:
        strResult = QString::fromUtf8("皮皮");
        break;

    case ZAO_CLASS_BAD2:
        strResult = QString::fromUtf8("裂口");
        break;

    case ZAO_CLASS_BAD3:
        strResult = QString::fromUtf8("鸟啄");
        break;

    case ZAO_CLASS_BAD4:
        strResult = QString::fromUtf8("烂枣");
        break;

    default:
        break;
    }

    return strResult;
}

void QAlgParasSetupDlg::onSelImgBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     ".",
                                                     tr("Images (*.png *.bmp *.jpg)"));

    QImage oldImage = QImage(fileName);
    if (oldImage.isNull())
        return;

    image = oldImage;
    plot->setImage(QPixmap::fromImage(image));
}

void QAlgParasSetupDlg::onTestBtnClicked()
{
    if (image.isNull())
        return;

    zaoInspectAlg.resetImageSize(image.width(), image.height());

    //更新检测参数
    updateInspectParas(zaoInspectAlg.getPInspectAlgParas());
    zaoInspectAlg.resetInspectParas();

    //检测
    cv::Mat mat = ImageTransform::QImageToMat(image);

    //检测图片及识别种类
    QVector<ZaoInfo> vecZaoInfo;
    int zaoCount = 0;
    if (zaoInspectAlg.zaoInspect(mat, vecZaoInfo, &zaoCount) < 0) {
        return;
    }

    QImage oldImage = image.copy();
    QPainter painter(&oldImage);
    QPen pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    painter.setFont(QFont("Arial", 30));
    for (int i = 0; i < zaoCount; i++) {
        painter.drawRect(vecZaoInfo[i].zaoPos.x, vecZaoInfo[i].zaoPos.y,
                  vecZaoInfo[i].zaoPos.width, vecZaoInfo[i].zaoPos.height);

        painter.drawText(QPoint(vecZaoInfo[i].zaoPos.x + 40, vecZaoInfo[i].zaoPos.y + 40),
                         getIdDesc(vecZaoInfo[i].classId));
    }

    plot->setImage(QPixmap::fromImage(oldImage));
}
