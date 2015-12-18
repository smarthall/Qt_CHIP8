#include "chip8keyboarddisplay.h"

#include <QPainter>
#include <string.h>

Chip8KeyboardDisplay::Chip8KeyboardDisplay(QWidget *parent) : QWidget(parent)
{
    memset(keys, 0, sizeof(keys));
}


void Chip8KeyboardDisplay::keyUp(uint8_t key_id)
{
    keyChange(false, key_id);
}

void Chip8KeyboardDisplay::keyDown(uint8_t key_id)
{
    keyChange(true, key_id);
}

void Chip8KeyboardDisplay::keyChange(bool pressed, uint8_t key_id)
{
    static const int keyMap[16] = {
        13, 0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 14, 3, 7, 11, 15
    };

    keys[keyMap[key_id]] = pressed;

    repaint();
}

void Chip8KeyboardDisplay::paintEvent(QPaintEvent *)
{
    static const char keyLabel[16] = {
        '1', '2', '3', 'C',
        '4', '5', '6', 'D',
        '7', '8', '9', 'E',
        'A', '0', 'B', 'F'
    };
    static const char keyMapLabel[16] = {
        '1', '2', '3', '4',
        'Q', 'W', 'E', 'R',
        'A', 'S', 'D', 'F',
        'Z', 'X', 'C', 'V'
    };

    // Make and configure a painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate the button sizes
    int buttonWidth = (width() - (5 * BTN_PADDING)) / 4;
    int buttonHeight = (height() - (5 * BTN_PADDING)) / 4;

    for (int i = 0; i < 16; i++) {
        // Calculate positioning
        int btn_x = i % 4;
        int btn_y = i / 4;
        int x = BTN_PADDING + (BTN_PADDING * btn_x) + (buttonWidth * btn_x);
        int y = BTN_PADDING + (BTN_PADDING * btn_y) + (buttonHeight * btn_y);
        QRect btn_rect(x, y, buttonWidth, buttonHeight);
        QRect text_rect(x + TEXT_PADDING,
                        y + TEXT_PADDING,
                        buttonWidth - (2 * TEXT_PADDING),
                        buttonHeight - (2 * TEXT_PADDING));

        // Get button data
        bool pressed = keys[i];
        QString chip8Label(keyLabel[i]);
        QString keyboardLabel(keyMapLabel[i]);

        // Setup the painter
        painter.setPen(QPen(Qt::black));
        if (pressed) {
            painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
        } else {
            painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        }

        // Draw the button
        painter.drawRoundedRect(btn_rect, BTN_PADDING, BTN_PADDING);

        // Setup the painter
        if (pressed) {
            painter.setPen(QPen(Qt::white));
        } else {
            painter.setPen(QPen(Qt::black));
        }

        // Draw the Keyboard Label
        QFont font = painter.font();
        font.setBold(true);
        font.setPixelSize(buttonHeight / 2);
        painter.setFont(font);
        painter.drawText(text_rect, Qt::AlignTop | Qt::AlignLeft, keyboardLabel);

        // Draw the CHIP8 Label
        font.setBold(false);
        font.setPixelSize(buttonHeight / 3);
        painter.setFont(font);
        painter.drawText(text_rect, Qt::AlignBottom | Qt::AlignRight, chip8Label);
    }
}


