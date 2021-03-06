#ifndef EMULATORDISPLAY_H
#define EMULATORDISPLAY_H

#include <QWidget>
#include <QSound>
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
    void playSound();

private:
    Ui::EmulatorDisplay *ui;
    EmulatorThread emulator;
    QSound *beep;
    void keyActionEvent(bool pressed, QKeyEvent *keyEvent);
    void passthroughKey(bool pressed, QKeyEvent *keyEvent);
};

#endif // EMULATORDISPLAY_H
