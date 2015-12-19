#include "emulatorwindow.h"
#include "ui_emulatordisplay.h"

#include <QDebug>
#include <QFile>
#include <QKeyEvent>
#include <QDirIterator>
#include <QStringList>

EmulatorWindow::EmulatorWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmulatorDisplay)
{
    ui->setupUi(this);

    // Disable resizing the window
    this->setWindowFlags(this->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // Load the Beep sound
    beep = new QSound(":/Sounds/beep.wav");

    // Connect the display to the emulator
    connect(&emulator, SIGNAL(displayUpdated()),
            this, SLOT(displayUpdated()),
            Qt::QueuedConnection);

    // Connect the keyboard to the emulator
    connect(this, SIGNAL(chip8KeyUp(uint8_t)),
            &emulator, SLOT(keyUp(uint8_t)));
    connect(this, SIGNAL(chip8KeyDown(uint8_t)),
            &emulator, SLOT(keyDown(uint8_t)));

    // Connect the sound player
    connect(&emulator, SIGNAL(playSound()),
            this, SLOT(playSound()),
            Qt::QueuedConnection);

    // Connect the keyboard to the GUI keys
    connect(this, SIGNAL(chip8KeyUp(uint8_t)),
            this->ui->keyboard, SLOT(keyUp(uint8_t)));
    connect(this, SIGNAL(chip8KeyDown(uint8_t)),
            this->ui->keyboard, SLOT(keyDown(uint8_t)));

    // Load the list of included ROMS
    QDirIterator it(":/ROMS/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        ui->romSelector->addItem(it.fileName(), it.filePath());
    }

    // Initialise the display
    displayUpdated();
}

EmulatorWindow::~EmulatorWindow()
{
    delete beep;
    delete ui;
}

void EmulatorWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    keyActionEvent(true, keyEvent);
}

void EmulatorWindow::keyReleaseEvent(QKeyEvent *keyEvent)
{
    keyActionEvent(false, keyEvent);
}

void EmulatorWindow::keyActionEvent(bool pressed, QKeyEvent *keyEvent) {
    // Key Mapping
    static const uint8_t key_count = 16;
    static const uint8_t key[key_count] = {
        Qt::Key_X,
        Qt::Key_1,
        Qt::Key_2,
        Qt::Key_3,
        Qt::Key_Q,
        Qt::Key_W,
        Qt::Key_E,
        Qt::Key_A,
        Qt::Key_S,
        Qt::Key_D,
        Qt::Key_Z,
        Qt::Key_C,
        Qt::Key_4,
        Qt::Key_R,
        Qt::Key_F,
        Qt::Key_V,
    };

    bool found = false;
    int i;

    // Is this an autorepeat event pass it through
    if (keyEvent->isAutoRepeat()) {
        passthroughKey(pressed, keyEvent);
        return;
    }

    // Look for a CHIP8 key
    for (i = 0; i < key_count; i++) {
        if (key[i] == keyEvent->key()) {
            found = true;
            break;
        }
    }

    if (!found) {
        passthroughKey(pressed, keyEvent);
        return;
    }

    if (pressed) {
        emit chip8KeyDown(i);
    } else {
        emit chip8KeyUp(i);
    }
}

void EmulatorWindow::passthroughKey(bool pressed, QKeyEvent *keyEvent) {
    if (pressed) {
        QWidget::keyPressEvent(keyEvent);
    } else {
        QWidget::keyReleaseEvent(keyEvent);
    }
}

void EmulatorWindow::displayUpdated() {
    uint8_t display[64 * 32];
    emulator.copyDisplay(display);
    ui->display->updateDisplay(display);
}

void EmulatorWindow::on_loadButton_clicked()
{
    // Find the path to the ROM
    int selected = ui->romSelector->currentIndex();
    QString chosen = ui->romSelector->itemData(selected).toString();

    // Stop the emulator
    emulator.stop();

    // Load a ROM
    QFile rom(chosen);
    emulator.load(rom);

    // Start it up
    emulator.start();
}

void EmulatorWindow::playSound()
{
    beep->play();
}
