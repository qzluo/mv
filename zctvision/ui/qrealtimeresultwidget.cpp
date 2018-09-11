#include "qrealtimeresultwidget.h"


QResultItemWidget::QResultItemWidget(QString name, QWidget *parent) : QWidget(parent)
{
    QFormLayout* sizeLayout = new QFormLayout();
    lengthLE = new QLineEdit(this);
    diameterLE = new QLineEdit(this);
    sizeLayout->addRow(tr("Length: "), lengthLE);
    sizeLayout->addRow(tr("Diameter: "), diameterLE);

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setSpacing(2);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(new QLabel(QString("#%1").arg(name)));
    resultLE = new QLineEdit(this);
    topLayout->addWidget(resultLE);
    topLayout->addLayout(sizeLayout);

    setLayout(topLayout);
}

void QResultItemWidget::setResult(QString &result, QString &length, QString &diameter)
{
    resultLE->setText(result);
    lengthLE->setText(length);
    diameterLE->setText(diameter);
}


///
/// \brief QRealTimeResultWidget::QRealTimeResultWidget
/// \param parent
///
QRealTimeResultWidget::QRealTimeResultWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* leftResLayout = new QVBoxLayout;
    QVBoxLayout* rightResLayout = new QVBoxLayout;

    for (int i = 1; i <= 4; ++i) {
        resultItems[i-1] = new QResultItemWidget(QString::number(i));
        resultItems[i+3] = new QResultItemWidget(QString::number(i+4));
        leftResLayout->addWidget(resultItems[i-1]);
        rightResLayout->addWidget(resultItems[i+3]);
    }

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setSpacing(2);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addLayout(leftResLayout);
    topLayout->addLayout(rightResLayout);

    setLayout(topLayout);
}

void QRealTimeResultWidget::setItemResult(int itemIndex, QString& result,
                                          QString& length, QString& diameter)
{
    if (itemIndex < 0 || itemIndex >= sizeof(resultItems)/sizeof(resultItems[0]))
        return;

    resultItems[itemIndex]->setResult(result, length, diameter);
}
