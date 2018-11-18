#ifndef QCAMERAPARASSETUPDLG_H
#define QCAMERAPARASSETUPDLG_H

#include <QDialog>
#include <QtWidgets>

#include "graphicswidget.h"
#include "cameractl.h"

class QCameraParasSetupDlg : public QDialog
{
    Q_OBJECT
public:
    //type = 0 -- vmba camera, type = 1 -- daheng camera
    explicit QCameraParasSetupDlg(int type = 0, QWidget *parent = 0);

    CameraCtl *getPCamCtl() const;
    void setPCamCtl(CameraCtl *value);

signals:

public slots:
    void onHasImage(const QImage& image);

    void onGainSliderValueChanged();
    void onGainSBValueChanged();

    void onExposureSBValueChanged();

    void onTriggerModeCBActivated();
    void onTriggerSourceCBActivated();
    void onTriggerDelaySBValueChanged();

    void onRedSBValueChanged();
    void onBlueSBValueChanged();

private:
    GraphicsWidget* plot;
    CameraCtl* pCamCtl;

    int cameraType;

    QSlider* gainSlider;
    QDoubleSpinBox* gainSB;

    QDoubleSpinBox* exposureSB;

    QComboBox* trigerModeCB;
    QComboBox* trigerSourceCB;
    QDoubleSpinBox* triggerDelaySB;

    QDoubleSpinBox* redSB;
    QDoubleSpinBox* blueSB;
};

#endif // QCAMERAPARASSETUPDLG_H
