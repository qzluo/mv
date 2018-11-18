#include "qcameraparassetupdlg.h"

QCameraParasSetupDlg::QCameraParasSetupDlg(int type, QWidget *parent) : QDialog(parent)
{
    pCamCtl = NULL;
    cameraType = type;

    //exposure
    QGroupBox* exposureGB = new QGroupBox(tr("Exposure"), this);
    QVBoxLayout* exposureGBLayout = new QVBoxLayout(exposureGB);

    exposureSB = new QDoubleSpinBox(this);

    connect(exposureSB, SIGNAL(valueChanged(double)),
            this, SLOT(onExposureSBValueChanged()));

    QHBoxLayout* exposureValueLayout = new QHBoxLayout();
    exposureValueLayout->addWidget(exposureSB);

    exposureGBLayout->addLayout(exposureValueLayout);

    //gain
    QGroupBox* gainGB = new QGroupBox(tr("Gain"), this);
    QVBoxLayout* gainGBLayout = new QVBoxLayout(gainGB);

    gainSlider = new QSlider(Qt::Horizontal, this);
    gainSB = new QDoubleSpinBox(this);

    connect(gainSlider, &QSlider::valueChanged,
            this, &QCameraParasSetupDlg::onGainSliderValueChanged);

    connect(gainSB, SIGNAL(valueChanged(double)),
            this, SLOT(onGainSBValueChanged()));

    QHBoxLayout* gainValueLayout = new QHBoxLayout();
    gainValueLayout->addWidget(gainSlider);
    gainValueLayout->addWidget(gainSB);

    gainGBLayout->addLayout(gainValueLayout);

    //trigger
    QGroupBox* triggerGB = new QGroupBox(tr("Trigger"), this);
    QFormLayout* triggerGBLayout = new QFormLayout(triggerGB);

    if (type == 2) { //daheng camera
        trigerModeCB = new QComboBox;
        connect(trigerModeCB, SIGNAL(activated(QString)),
                this, SLOT(onTriggerModeCBActivated()));
        triggerGBLayout->addRow(tr("Mode"), trigerModeCB);
    }

    trigerSourceCB = new QComboBox;
    connect(trigerSourceCB, SIGNAL(activated(QString)),
            this, SLOT(onTriggerSourceCBActivated()));

    triggerDelaySB = new QDoubleSpinBox;
    connect(triggerDelaySB, SIGNAL(valueChanged(double)),
            this, SLOT(onTriggerDelaySBValueChanged()));

    triggerGBLayout->addRow(tr("Source"), trigerSourceCB);
    triggerGBLayout->addRow(tr("Trigger Delay"), triggerDelaySB);



    //param
    QFrame* paramFrame = new QFrame(this);
    paramFrame->setFrameShape(QFrame::Box);
    paramFrame->setFrameShadow(QFrame::Raised);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(paramFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QVBoxLayout* parasSetupLayout = new QVBoxLayout(paramFrame);
    parasSetupLayout->addWidget(exposureGB);
    parasSetupLayout->addWidget(gainGB);
    parasSetupLayout->addWidget(triggerGB);

    //white balance
    if (type == 1) {
        QGroupBox* whiteBalanceGB = new QGroupBox(tr("White Balance"), this);
        QFormLayout* whiteBalanceGBLayout = new QFormLayout(whiteBalanceGB);

        redSB = new QDoubleSpinBox(this);
        blueSB = new QDoubleSpinBox(this);

        connect(redSB, SIGNAL(valueChanged(double)),
                this, SLOT(onRedSBValueChanged()));
        connect(blueSB, SIGNAL(valueChanged(double)),
                this, SLOT(onBlueSBValueChanged()));

        whiteBalanceGBLayout->addRow(tr("Red"), redSB);
        whiteBalanceGBLayout->addRow(tr("Blue"), blueSB);

        parasSetupLayout->addWidget(whiteBalanceGB);
    }

    parasSetupLayout->addStretch();

    plot = new GraphicsWidget(this);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(plot);
    topLayout->addWidget(scrollArea);

    setLayout(topLayout);
    setWindowTitle(tr("Camera Parameters Setup"));

    QRect screenRect = QApplication::desktop()->screenGeometry();
    resize(screenRect.width()-100,screenRect.height()-100);
    setWindowState(Qt::WindowActive/* Qt::WindowMaximized*/);
}

CameraCtl *QCameraParasSetupDlg::getPCamCtl() const
{
    return pCamCtl;
}

void QCameraParasSetupDlg::setPCamCtl(CameraCtl *value)
{
    pCamCtl = value;

    connect(pCamCtl, &CameraCtl::hasImage,
            this, &QCameraParasSetupDlg::onHasImage);

    CameraFeature camearaFeature = {};

    //exposure
    if (pCamCtl->GetPara("ExposureTimeAbs", &camearaFeature) == 0) {
        double dMin = camearaFeature.min.toDouble();
        double dMax = camearaFeature.max.toDouble();
        double dValue = camearaFeature.value.toDouble();

        exposureSB->setRange(dMin, dMax);
        exposureSB->setValue(dValue);
    }

    //get gain
    if (pCamCtl->GetPara("Gain", &camearaFeature) == 0) {
        double dMin = camearaFeature.min.toDouble();
        double dMax = camearaFeature.max.toDouble();
        double dValue = camearaFeature.value.toDouble();
        gainSlider->setMinimum(dMin);
        gainSlider->setMaximum(dMax);
        gainSlider->setValue(dValue);

        gainSB->setRange(dMin, dMax);
        gainSB->setValue(dValue);
    }

    //trigger
    if (cameraType == 2) {
        if (pCamCtl->GetPara("TriggerMode", &camearaFeature) == 0) {
            trigerModeCB->clear();

            for (int i = 0; i < camearaFeature.enumStrings.size(); ++i) {
                trigerModeCB->addItem(camearaFeature.enumStrings.at(i));
            }

            if (trigerModeCB->findText(camearaFeature.value) >= 0)
                trigerModeCB->setCurrentText(camearaFeature.value);
        }
    }

    if (pCamCtl->GetPara("TriggerSource", &camearaFeature) == 0) {
        trigerSourceCB->clear();

        for (int i = 0; i < camearaFeature.enumStrings.size(); ++i) {
            trigerSourceCB->addItem(camearaFeature.enumStrings.at(i));
        }

        if (trigerSourceCB->findText(camearaFeature.value) >= 0)
            trigerSourceCB->setCurrentText(camearaFeature.value);
    }

    if (pCamCtl->GetPara("TriggerDelayAbs", &camearaFeature) == 0) {
        double dMin = camearaFeature.min.toDouble();
        double dMax = camearaFeature.max.toDouble();
        double dValue = camearaFeature.value.toDouble();

        triggerDelaySB->setRange(dMin, dMax);
        triggerDelaySB->setValue(dValue);
    }

    //white balance
    if (cameraType == 1) {
        if (pCamCtl->SetPara("BalanceRatioSelector", "Red") == 0) {
            if (pCamCtl->GetPara("BalanceRatioAbs", &camearaFeature) == 0) {
                double dMin = camearaFeature.min.toDouble();
                double dMax = camearaFeature.max.toDouble();
                double dValue = camearaFeature.value.toDouble();

                redSB->setRange(dMin, dMax);
                redSB->setSingleStep(0.1);
                redSB->setValue(dValue);
            }
        }

        if (pCamCtl->SetPara("BalanceRatioSelector", "Blue") == 0) {
            if (pCamCtl->GetPara("BalanceRatioAbs", &camearaFeature) == 0) {
                double dMin = camearaFeature.min.toDouble();
                double dMax = camearaFeature.max.toDouble();
                double dValue = camearaFeature.value.toDouble();

                blueSB->setRange(dMin, dMax);
                blueSB->setSingleStep(0.1);
                blueSB->setValue(dValue);
            }
        }
    }
}

void QCameraParasSetupDlg::onHasImage(const QImage &image)
{
    plot->setImage((QPixmap::fromImage(image)));
}

void QCameraParasSetupDlg::onGainSliderValueChanged()
{
    double dValue = gainSlider->value();
    gainSB->setValue(dValue);

    pCamCtl->SetPara("Gain", QString::number(dValue));
}

void QCameraParasSetupDlg::onGainSBValueChanged()
{
    double dValue = gainSB->value();
    gainSlider->setValue(dValue);

    pCamCtl->SetPara("Gain", QString::number(dValue));
}

void QCameraParasSetupDlg::onExposureSBValueChanged()
{
    double dValue = exposureSB->value();
    pCamCtl->SetPara("ExposureTimeAbs", QString::number(dValue));
}

void QCameraParasSetupDlg::onTriggerModeCBActivated()
{
    QString curTS = trigerModeCB->currentText();
    pCamCtl->SetPara("TriggerMode", curTS);
}

void QCameraParasSetupDlg::onTriggerSourceCBActivated()
{
    QString curTS = trigerSourceCB->currentText();
    pCamCtl->SetPara("TriggerSource", curTS);
}

void QCameraParasSetupDlg::onTriggerDelaySBValueChanged()
{
    double dValue = triggerDelaySB->value();
    pCamCtl->SetPara("TriggerDelayAbs", QString::number(dValue));
}

void QCameraParasSetupDlg::onRedSBValueChanged()
{
    if (pCamCtl->SetPara("BalanceRatioSelector", "Red") == 0) {
        double dValue = redSB->value();
        pCamCtl->SetPara("BalanceRatioAbs", QString::number(dValue));
    }
}

void QCameraParasSetupDlg::onBlueSBValueChanged()
{
    if (pCamCtl->SetPara("BalanceRatioSelector", "Blue") == 0) {
        double dValue = blueSB->value();
        pCamCtl->SetPara("BalanceRatioAbs", QString::number(dValue));
    }
}
