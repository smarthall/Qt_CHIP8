#ifndef CHIP8KEYBOARDDISPLAY_H
#define CHIP8KEYBOARDDISPLAY_H

#include <QWidget>

class Chip8KeyboardDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit Chip8KeyboardDisplay(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // CHIP8KEYBOARDDISPLAY_H
