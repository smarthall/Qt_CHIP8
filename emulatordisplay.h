#ifndef EMULATORDISPLAY_H
#define EMULATORDISPLAY_H

#include <QWidget>

#include "chip8core.h"

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

private:
    Ui::EmulatorDisplay *ui;
    Chip8Core emulator;
};

#endif // EMULATORDISPLAY_H
