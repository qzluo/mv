#include "qctrlboardparassetupdlg.h"

QCtrlBoardParasSetupDlg::QCtrlBoardParasSetupDlg(QWidget *parent) : QDialog(parent)
{
    QGridLayout* registerContsLayout = new QGridLayout;
    int row = 0;
    int col = 0;
    QLabel* regiserLabels[8] = {};
    for (int i = 0; i < 8; ++i) {
        regiserLabels[i] = new QLabel(QString(tr("Register#%1")).arg(i+1));
        registerConts[i] = new QLineEdit();
        registerContsLayout->addItem(row, col++, regiserLabels[i]);
        registerContsLayout->addItem(row, col++, registerConts[i]);

        if (col == 4) {
            row++;
            col = 0;
        }
    }

    QPushButton* readRegisterBtn = new QPushButton;
    QHBoxLayout* registerLayout = new QHBoxLayout;
    registerLayout->addLayout(registerContsLayout);
    registerLayout->addItem(readRegisterBtn);

    QVBoxLayout* topLayout = new QVBoxLayout;

    topLayout->addLayout(registerLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Controller Board Parameters"));
}
