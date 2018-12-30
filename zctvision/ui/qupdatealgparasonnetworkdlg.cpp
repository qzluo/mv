#include "qupdatealgparasonnetworkdlg.h"
#include <QJsonDocument>

#include "mainresource.h"

QUpdateAlgParasOnNetworkDlg::QUpdateAlgParasOnNetworkDlg(QWidget *parent) : QDialog(parent)
{
    pInspectAlgParas = NULL;

    //Good Jujube Parameters
    //1
    QLabel* class1LengthLbl = new QLabel(tr("Class1 Length: "));
    class1LengthDSB = new QDoubleSpinBox(this);
    class1LengthDSB->setRange(0, 10000);

    QLabel* class1WidthLbl = new QLabel(tr("Class1 Width: "));
    class1WidthDSB = new QDoubleSpinBox(this);
    class1WidthDSB->setRange(0, 10000);

    //2
    QLabel* class2LengthLbl = new QLabel(tr("Class2 Length: "));
    class2LengthDSB = new QDoubleSpinBox(this);
    class2LengthDSB->setRange(0, 10000);

    QLabel* class2WidthLbl = new QLabel(tr("Class2 Width: "));
    class2WidthDSB = new QDoubleSpinBox(this);
    class2WidthDSB->setRange(0, 10000);

    //3
    QLabel* class3LengthLbl = new QLabel(tr("Class3 Length: "));
    class3LengthDSB = new QDoubleSpinBox(this);
    class3LengthDSB->setRange(0, 10000);

    QLabel* class3WidthLbl = new QLabel(tr("Class3 Width: "));
    class3WidthDSB = new QDoubleSpinBox(this);
    class3WidthDSB->setRange(0, 10000);

    //detect parameter
    //I
    QLabel* iMinLbl = new QLabel(tr("Imin: "));
    iMinSB = new QSpinBox(this);
    iMinSB->setRange(0, 255);

    QLabel* iMaxLbl = new QLabel(tr("Imax: "));
    iMaxSB = new QSpinBox(this);
    iMaxSB->setRange(0, 255);

    //B
    QLabel* bMinLbl = new QLabel(tr("Bmin: "));
    bMinSB = new QSpinBox(this);
    bMinSB->setRange(0, 255);

    QLabel* bMaxLbl = new QLabel(tr("Bmax: "));
    bMaxSB = new QSpinBox(this);
    bMaxSB->setRange(0, 255);

    //R
    QLabel* rMinLbl = new QLabel(tr("Rmin: "));
    rMinSB = new QSpinBox(this);
    rMinSB->setRange(0, 255);

    QLabel* rMaxLbl = new QLabel(tr("Rmax: "));
    rMaxSB = new QSpinBox(this);
    rMaxSB->setRange(0, 255);

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

    //new detect parameters
    old_class1LengthDSB = new QLabel(this);
    old_class1WidthDSB = new QLabel(this);
    old_class2LengthDSB = new QLabel(this);
    old_class2WidthDSB = new QLabel(this);
    old_class3LengthDSB = new QLabel(this);
    old_class3WidthDSB = new QLabel(this);

    old_iMinSB = new QLabel(this);
    old_iMaxSB = new QLabel(this);
    old_bMinSB = new QLabel(this);
    old_bMaxSB = new QLabel(this);
    old_rMinSB = new QLabel(this);
    old_rMaxSB = new QLabel(this);

    old_wMinSB = new QLabel(this);
    old_hMinSB = new QLabel(this);
    old_ratioMaxSB = new QLabel(this);

    //param
    QFrame* paramFrame = new QFrame(this);
    paramFrame->setFrameShape(QFrame::Box);
    paramFrame->setFrameShadow(QFrame::Raised);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(paramFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QVBoxLayout* scrollLayout = new QVBoxLayout(paramFrame);

    QGridLayout* parasSetupLayout = new QGridLayout();
    parasSetupLayout->setSpacing(10);
    parasSetupLayout->addWidget(new QLabel(tr("Parameters")), 0, 0);
    parasSetupLayout->addWidget(new QLabel(tr("New Value")), 0, 2);
    parasSetupLayout->addWidget(new QLabel(tr("Old Value")), 0, 1);

    //class1Length
    parasSetupLayout->addWidget(class1LengthLbl, 1, 0);
    parasSetupLayout->addWidget(class1LengthDSB, 1, 2);
    parasSetupLayout->addWidget(old_class1LengthDSB, 1, 1);

    //class1Width
    parasSetupLayout->addWidget(class1WidthLbl, 2, 0);
    parasSetupLayout->addWidget(class1WidthDSB, 2, 2);
    parasSetupLayout->addWidget(old_class1WidthDSB, 2, 1);

    //class2Length
    parasSetupLayout->addWidget(class2LengthLbl, 3, 0);
    parasSetupLayout->addWidget(class2LengthDSB, 3, 2);
    parasSetupLayout->addWidget(old_class2LengthDSB, 3, 1);

    //class2Width
    parasSetupLayout->addWidget(class2WidthLbl, 4, 0);
    parasSetupLayout->addWidget(class2WidthDSB, 4, 2);
    parasSetupLayout->addWidget(old_class2WidthDSB, 4, 1);

    //class3Length
    parasSetupLayout->addWidget(class3LengthLbl, 5, 0);
    parasSetupLayout->addWidget(class3LengthDSB, 5, 2);
    parasSetupLayout->addWidget(old_class3LengthDSB, 5, 1);

    //class3Width
    parasSetupLayout->addWidget(class3WidthLbl, 6, 0);
    parasSetupLayout->addWidget(class3WidthDSB, 6, 2);
    parasSetupLayout->addWidget(old_class3WidthDSB, 6, 1);

    //iMin
    parasSetupLayout->addWidget(iMinLbl, 7, 0);
    parasSetupLayout->addWidget(iMinSB, 7, 2);
    parasSetupLayout->addWidget(old_iMinSB, 7, 1);

    //iMax
    parasSetupLayout->addWidget(iMaxLbl, 8, 0);
    parasSetupLayout->addWidget(iMaxSB, 8, 2);
    parasSetupLayout->addWidget(old_iMaxSB, 8, 1);

    //bMin
    parasSetupLayout->addWidget(bMinLbl, 9, 0);
    parasSetupLayout->addWidget(bMinSB, 9, 2);
    parasSetupLayout->addWidget(old_bMinSB, 9, 1);

    //bMax
    parasSetupLayout->addWidget(bMaxLbl, 10, 0);
    parasSetupLayout->addWidget(bMaxSB, 10, 2);
    parasSetupLayout->addWidget(old_bMaxSB, 10, 1);

    //rMin
    parasSetupLayout->addWidget(rMinLbl, 11, 0);
    parasSetupLayout->addWidget(rMinSB, 11, 2);
    parasSetupLayout->addWidget(old_rMinSB, 11, 1);

    //rMax
    parasSetupLayout->addWidget(rMaxLbl, 12, 0);
    parasSetupLayout->addWidget(rMaxSB, 12, 2);
    parasSetupLayout->addWidget(old_rMaxSB, 12, 1);

    //wMin
    parasSetupLayout->addWidget(wMinLbl, 13, 0);
    parasSetupLayout->addWidget(wMinSB, 13, 2);
    parasSetupLayout->addWidget(old_wMinSB, 13, 1);

    //hMin
    parasSetupLayout->addWidget(hMinLbl, 14, 0);
    parasSetupLayout->addWidget(hMinSB, 14, 2);
    parasSetupLayout->addWidget(old_hMinSB, 14, 1);

    //ratioMax
    parasSetupLayout->addWidget(ratioMaxLbl, 15, 0);
    parasSetupLayout->addWidget(ratioMaxSB, 15, 2);
    parasSetupLayout->addWidget(old_ratioMaxSB, 15, 1);

    scrollLayout->addLayout(parasSetupLayout);
    scrollLayout->addStretch();

    //btns
    QPushButton* okBtn = new QPushButton(tr("Update"), this);
    QPushButton* cancelBtn = new QPushButton(tr("Exit"), this);
    connect(okBtn, &QPushButton::clicked,
            this, &QUpdateAlgParasOnNetworkDlg::onOkBtnClicked);
    connect(cancelBtn, &QPushButton::clicked,
            this, &QUpdateAlgParasOnNetworkDlg::reject);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->addWidget(scrollArea);
    topLayout->addLayout(btnLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Update Algrithm Parameters"));

    QRect screenRect = QApplication::desktop()->screenGeometry();
    resize(screenRect.width() / 4, screenRect.height()-200);
    setWindowState(Qt::WindowActive/* Qt::WindowMaximized*/);
}

QZaoInspectAlgParas *QUpdateAlgParasOnNetworkDlg::getPInspectAlgParas() const
{
    return pInspectAlgParas;
}

void QUpdateAlgParasOnNetworkDlg::setPInspectAlgParas(QZaoInspectAlgParas *value)
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

    old_class1LengthDSB->setText(class1LengthDSB->text());
    old_class1WidthDSB->setText(class1WidthDSB->text());
    old_class2LengthDSB->setText(class2LengthDSB->text());
    old_class2WidthDSB->setText(class2WidthDSB->text());
    old_class3LengthDSB->setText(class3LengthDSB->text());
    old_class3WidthDSB->setText(class3WidthDSB->text());

    old_iMinSB->setText(iMinSB->text());
    old_iMaxSB->setText(iMaxSB->text());
    old_bMinSB->setText(bMinSB->text());
    old_bMaxSB->setText(bMaxSB->text());
    old_rMinSB->setText(rMinSB->text());
    old_rMaxSB->setText(rMaxSB->text());

    old_wMinSB->setText(wMinSB->text());
    old_hMinSB->setText(hMinSB->text());
    old_ratioMaxSB->setText(ratioMaxSB->text());

    //new value
    old_class1Length = class1LengthDSB->value();
    old_class1Width = class1WidthDSB->value();
    old_class2Length = class2LengthDSB->value();
    old_class2Width = class2WidthDSB->value();
    old_class3Length = class3LengthDSB->value();
    old_class3Width = class3WidthDSB->value();
    old_iMin = iMinSB->value();
    old_iMax = iMaxSB->value();
    old_bMin = bMinSB->value();
    old_bMax = bMaxSB->value();
    old_rMin = rMinSB->value();
    old_rMax = rMaxSB->value();
    old_wMin = wMinSB->value();
    old_hMin = hMinSB->value();
    old_ratioMax = ratioMaxSB->value();
}

void QUpdateAlgParasOnNetworkDlg::onOkBtnClicked()
{
    if (!pInspectAlgParas)
        return;

    //new value
    double new_class1Length = class1LengthDSB->value();
    double new_class1Width = class1WidthDSB->value();
    double new_class2Length = class2LengthDSB->value();
    double new_class2Width = class2WidthDSB->value();
    double new_class3Length = class3LengthDSB->value();
    double new_class3Width = class3WidthDSB->value();
    int new_iMin = iMinSB->value();
    int new_iMax = iMaxSB->value();
    int new_bMin = bMinSB->value();
    int new_bMax = bMaxSB->value();
    int new_rMin = rMinSB->value();
    int new_rMax = rMaxSB->value();
    int new_wMin = wMinSB->value();
    int new_hMin = hMinSB->value();
    double new_ratioMax = ratioMaxSB->value();

    bool bChanged = false;
    //生成增量json
    QJsonObject json;
    if (new_class1Length != old_class1Length) {
        json.insert("class_good1_length", new_class1Length - old_class1Length);
        pInspectAlgParas->setClass_good1_length(new_class1Length);
        old_class1LengthDSB->setText(class1LengthDSB->text());
        old_class1Length = new_class1Length;
        bChanged = true;
    }

    if (new_class1Width != old_class1Width) {
        json.insert("class_good1_width", new_class1Width - old_class1Width);
        pInspectAlgParas->setClass_good1_width(new_class1Width);
        old_class1WidthDSB->setText(class1WidthDSB->text());
        old_class1Width = new_class1Width;
        bChanged = true;
    }

    if (new_class2Length != old_class2Length) {
        json.insert("class_good2_length", new_class2Length - old_class2Length);
        pInspectAlgParas->setClass_good2_length(new_class2Length);
        old_class2LengthDSB->setText(class2LengthDSB->text());
        old_class2Length = new_class2Length;
        bChanged = true;
    }

    if (new_class2Width != old_class2Width) {
        json.insert("class_good2_width", new_class2Width - old_class2Width);
        pInspectAlgParas->setClass_good2_width(new_class2Width);
        old_class2WidthDSB->setText(class2WidthDSB->text());
        old_class2Width = new_class2Width;
        bChanged = true;
    }

    if (new_class3Length != old_class3Length) {
        json.insert("class_good3_length", new_class3Length - old_class3Length);
        pInspectAlgParas->setClass_good3_length(new_class3Length);
        old_class3LengthDSB->setText(class3LengthDSB->text());
        old_class3Length = new_class3Length;
        bChanged = true;
    }

    if (new_class3Width != old_class3Width) {
        json.insert("class_good3_width", new_class3Width - old_class3Width);
        pInspectAlgParas->setClass_good3_width(new_class3Width);
        old_class3WidthDSB->setText(class3WidthDSB->text());
        old_class3Width = new_class3Width;
        bChanged = true;
    }

    if (new_iMin != old_iMin) {
        json.insert("Imin", new_iMin - old_iMin);
        pInspectAlgParas->setImin(new_iMin);
        old_iMinSB->setText(iMinSB->text());
        old_iMin = new_iMin;
        bChanged = true;
    }

    if (new_bMin != old_bMin) {
        json.insert("Bmin", new_bMin - old_bMin);
        pInspectAlgParas->setBmin(new_bMin);
        old_bMinSB->setText(bMinSB->text());
        old_bMin = new_bMin;
        bChanged = true;
    }

    if (new_rMin != old_rMin) {
        json.insert("Rmin", new_rMin - old_rMin);
        pInspectAlgParas->setRmin(new_rMin);
        old_rMinSB->setText(rMinSB->text());
        old_rMin = new_rMin;
        bChanged = true;
    }

    if (new_iMax != old_iMax) {
        json.insert("Imax", new_iMax - old_iMax);
        pInspectAlgParas->setImax(new_iMax);
        old_iMaxSB->setText(iMaxSB->text());
        old_iMax = new_iMax;
        bChanged = true;
    }

    if (new_bMax != old_bMax) {
        json.insert("Bmax", new_bMax - old_bMax);
        pInspectAlgParas->setBmax(new_bMax);
        old_bMaxSB->setText(bMaxSB->text());
        old_bMax = new_bMax;
        bChanged = true;
    }

    if (new_rMax != old_rMax) {
        json.insert("Rmax", new_rMax - old_rMax);
        pInspectAlgParas->setRmax(new_rMax);
        old_rMaxSB->setText(rMaxSB->text());
        old_rMax = new_rMax;
        bChanged = true;
    }

    if (new_wMin != old_wMin) {
        json.insert("Wmin", new_wMin - old_wMin);
        pInspectAlgParas->setWmin(new_wMin);
        old_wMinSB->setText(wMinSB->text());
        old_wMin = new_wMin;
        bChanged = true;
    }

    if (new_hMin != old_hMin) {
        json.insert("Hmin", new_hMin - old_hMin);
        pInspectAlgParas->setRatioMax(new_ratioMax);
        old_hMinSB->setText(hMinSB->text());
        old_hMin = new_hMin;
        bChanged = true;
    }

    if (new_ratioMax != old_ratioMax) {
        json.insert("RatioMax", new_ratioMax - old_ratioMax);
        pInspectAlgParas->setHmin(new_hMin);
        old_ratioMaxSB->setText(ratioMaxSB->text());
        old_ratioMax = new_ratioMax;
        bChanged = true;
    }

    if (bChanged) {
        //send upd data
        QJsonDocument document;
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        mic->broadcastUdpDatagram(byte_array.data(), byte_array.size());

        pInspectAlgParas->save();
    }
}
