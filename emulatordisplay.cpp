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

    emulator.reset();
    emulator.load((uint8_t*)program.constData(), program.size());

    // Copy the initial display
    displayUpdated();

    // Start the emulator thread
    emu_thread = new EmulatorThread(&emulator);
    connect(emu_thread, SIGNAL(EmulatorThread::displayUpdated),
            ui->display, SLOT(EmulatorDisplay::displayUpdated),
            Qt::QueuedConnection);
    emu_thread->start();
}

EmulatorDisplay::~EmulatorDisplay()
{
    delete emu_thread;
    delete ui;
}

void EmulatorDisplay::displayUpdated() {
    uint8_t display[64 * 32];
    emulator.copyDisplay(display);
    ui->display->updateDisplay(display);
}
