#include "emulatordisplay.h"
#include "ui_emulatordisplay.h"

EmulatorDisplay::EmulatorDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmulatorDisplay)
{
    ui->setupUi(this);
}

EmulatorDisplay::~EmulatorDisplay()
{
    delete ui;
}
