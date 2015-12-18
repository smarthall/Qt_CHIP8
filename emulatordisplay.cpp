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

    // Connect the display
    connect(&emulator, SIGNAL(displayUpdated()),
            this, SLOT(displayUpdated()),
            Qt::QueuedConnection);

    // Load a program
    QFile rom(":/ROMS/INVADERS");
    emulator.load(rom);

    // Start it up
    emulator.start();
}

EmulatorDisplay::~EmulatorDisplay()
{
    delete ui;
}

void EmulatorDisplay::displayUpdated() {
    uint8_t display[64 * 32];
    emulator.copyDisplay(display);
    ui->display->updateDisplay(display);
}
