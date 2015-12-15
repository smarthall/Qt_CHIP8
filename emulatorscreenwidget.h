#ifndef EMULATORSCREENWIDGET_H
#define EMULATORSCREENWIDGET_H

#include <QWidget>
#include <inttypes.h>

class EmulatorScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmulatorScreenWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void updateDisplay(uint8_t *display);

private:
    uint8_t display[64 * 32];

signals:

public slots:
};

#endif // EMULATORSCREENWIDGET_H
