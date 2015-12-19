#ifndef EMULATORDISPLAY_H
#define EMULATORDISPLAY_H

#include <QWidget>
#include <inttypes.h>

#include "chip8core.h"
#include "emulatorthread.h"

namespace Ui {
class EmulatorDisplay;
}

class EmulatorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EmulatorWindow(QWidget *parent = 0);
    ~EmulatorWindow();
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);

signals:
    void chip8KeyUp(uint8_t key);
    void chip8KeyDown(uint8_t key);

private slots:
    void displayUpdated();

    void on_loadButton_clicked();

private:
    Ui::EmulatorDisplay *ui;
    EmulatorThread emulator;
    void keyActionEvent(bool pressed, QKeyEvent *keyEvent);
    void passthroughKey(bool pressed, QKeyEvent *keyEvent);
};

#endif // EMULATORDISPLAY_H
