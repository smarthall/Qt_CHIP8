#include "emulatordisplay.h"
#include "ui_emulatordisplay.h"

EmulatorDisplay::EmulatorDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmulatorDisplay)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

EmulatorDisplay::~EmulatorDisplay()
{
    delete ui;
}
