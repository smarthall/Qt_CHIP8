#include "chip8keyboarddisplay.h"

#include <QPainter>

Chip8KeyboardDisplay::Chip8KeyboardDisplay(QWidget *parent) : QWidget(parent)
{

}

void Chip8KeyboardDisplay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
}

