#include "emulatordisplay.h"
#include "ui_emulatordisplay.h"

#include <QDebug>

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

void EmulatorDisplay::on_resetButton_clicked()
{
    qDebug() << "Click!";
}
