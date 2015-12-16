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
    QRgb white = qRgb(255, 255, 255);

    int width = this->width();
    int height = this->height();
    double x_scale = (double)this->display_size_x / width;
    double y_scale = (double)this->display_size_y / height;

    QRgb* pixels = new QRgb[width * height];
    QImage image((uchar*)pixels, width, height, QImage::Format_ARGB32);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int scaled_x = x * x_scale;
            int scaled_y = y * y_scale;
            pixels[x + y * width] = getChip8Pixel(scaled_x, scaled_y)? white : black;
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

unsigned int EmulatorScreenWidget::getChip8Pixel(unsigned int x, unsigned int y) {
    return this->display[x + y * this->display_size_x];
}
