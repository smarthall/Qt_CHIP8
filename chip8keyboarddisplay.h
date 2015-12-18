#ifndef CHIP8KEYBOARDDISPLAY_H
#define CHIP8KEYBOARDDISPLAY_H

#include <QWidget>
#include <inttypes.h>

class Chip8KeyboardDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit Chip8KeyboardDisplay(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void keyUp(uint8_t key_id);
    void keyDown(uint8_t key_id);

private:
    void keyChange(bool pressed, uint8_t key_id);
    static const int BTN_PADDING = 10;
    static const int TEXT_PADDING = 4;
    bool keys[16];
};

#endif // CHIP8KEYBOARDDISPLAY_H
