#ifndef EMULATORDISPLAY_H
#define EMULATORDISPLAY_H

#include <QWidget>

#include "chip8core.h"
#include "emulatorthread.h"

namespace Ui {
class EmulatorDisplay;
}

class EmulatorDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit EmulatorDisplay(QWidget *parent = 0);
    ~EmulatorDisplay();

private slots:
    void displayUpdated();

private:
    Ui::EmulatorDisplay *ui;
    EmulatorThread *emu_thread;
    Chip8Core emulator;
};

#endif // EMULATORDISPLAY_H
