#include "emulatordisplay.h"
#include "ui_emulatordisplay.h"

#include <QDebug>
#include <QFile>

EmulatorDisplay::EmulatorDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmulatorDisplay)
{
    ui->setupUi(this);

    // Disable resizing the window
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // Load a program
    QFile rom(":/ROMS/INVADERS");

    if (!rom.open(QIODevice::ReadOnly))
        return;

    QByteArray program = rom.read(emulator.MAX_PROGRAM_SIZE);

    emulator.load((uint8_t*)program.constData(), program.size());

    // Copy the initial display
    uint8_t display[64 * 32];
    emulator.copyDisplay(display);
    ui->display->updateDisplay(display);
}

EmulatorDisplay::~EmulatorDisplay()
{
    delete ui;
}

