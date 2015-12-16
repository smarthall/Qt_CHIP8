#ifndef EMULATORSCREENWIDGET_H
#define EMULATORSCREENWIDGET_H

#include <QWidget>
#include <inttypes.h>

class EmulatorScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmulatorScreenWidget(QWidget *parent = 0);
    void updateDisplay(uint8_t *display);

protected:
    void paintEvent(QPaintEvent *event);

private:
    static const int display_size_x = 64;
    static const int display_size_y = 32;
    uint8_t display[display_size_x * display_size_y];

signals:

public slots:
};

#endif // EMULATORSCREENWIDGET_H
