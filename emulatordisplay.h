#ifndef EMULATORDISPLAY_H
#define EMULATORDISPLAY_H

#include <QWidget>

namespace Ui {
class EmulatorDisplay;
}

class EmulatorDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit EmulatorDisplay(QWidget *parent = 0);
    ~EmulatorDisplay();

private:
    Ui::EmulatorDisplay *ui;
};

#endif // EMULATORDISPLAY_H
