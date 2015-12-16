#include "emulatorscreenwidget.h"

#include <Qtime>
#include <QDebug>
#include <QPainter>

EmulatorScreenWidget::EmulatorScreenWidget(QWidget *parent) : QWidget(parent) {
    qDebug() << "Widget created";
}

void EmulatorScreenWidget::paintEvent(QPaintEvent*) {
    QTime time;
    time.start();
    QPainter painter(this);
    QRgb black = qRgb(0, 0, 0);

    QRgb* pixels = new QRgb[this->display_size_x * this->display_size_y];
    QImage image((uchar*)pixels, this->display_size_x, this->display_size_y, QImage::Format_ARGB32);

    for (int x = 0; x < this->display_size_x; ++x) {
        for (int y = 0; y < this->display_size_y; ++y) {
            pixels[x + y * this->display_size_x] = black;
        }
    }
    painter.drawImage(0, 0, image);
    qDebug() << "Drew frame in :" << time.elapsed() << "ms.";
    delete[] pixels;
}

void EmulatorScreenWidget::updateDisplay(uint8_t *display) {
    memcpy(this->display, display, sizeof(this->display));

    this->repaint();
}
