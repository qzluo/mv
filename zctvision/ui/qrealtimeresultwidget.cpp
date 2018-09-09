#include "qrealtimeresultwidget.h"


QResultItemWidget::QResultItemWidget(QString name, QWidget *parent) : QWidget(parent)
{
    QFormLayout* sizeLayout = new QFormLayout();
    sizeLayout->addRow(tr("Length: "), new QLineEdit());
    sizeLayout->addRow(tr("Diameter: "), new QLineEdit());

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addWidget(new QLabel(QString("#%1").arg(name)));
    topLayout->addWidget(new QLineEdit());
    topLayout->addLayout(sizeLayout);

    setLayout(topLayout);

    resize(200, 80);
}


///
/// \brief QRealTimeResultWidget::QRealTimeResultWidget
/// \param parent
///
QRealTimeResultWidget::QRealTimeResultWidget(QWidget *parent) : QWidget(parent)
{
//    QVBoxLayout* leftResLayout = new QVBoxLayout;
//    QVBoxLayout* rightResLayout = new QVBoxLayout;

//    for (int i = 1; i <= 4; ++i) {
//        leftResLayout->addWidget(new QResultItemWidget(QString::number(i)));
//        rightResLayout->addWidget(new QResultItemWidget(QString::number(i+4)));
//    }

//    QHBoxLayout* topLayout = new QHBoxLayout(this);
//    topLayout->addLayout(leftResLayout);
//    topLayout->addLayout(rightResLayout);

    QVBoxLayout* leftResLayout = new QVBoxLayout;
    QVBoxLayout* rightResLayout = new QVBoxLayout;

    for (int i = 1; i <= 4; ++i) {
        leftResLayout->addWidget(new QLabel(QString::number(i)));
        rightResLayout->addWidget(new QLabel(QString::number(i+4)));
    }

    QHBoxLayout* topLayout = new QHBoxLayout(this);
    topLayout->addLayout(leftResLayout);
    topLayout->addLayout(rightResLayout);

}
