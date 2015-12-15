#include "emulatorscreenwidget.h"

#include <Qtime>
#include <QDebug>
#include <QPainter>

EmulatorScreenWidget::EmulatorScreenWidget(QWidget *parent) : QWidget(parent) {

}

void EmulatorScreenWidget::paintEvent(QPaintEvent*) {
    QRgb* pixels = new QRgb[width()*height()];
    QTime time;
    time.start();
    QPainter painter(this);
    QImage image((uchar*)pixels, width(), height(), QImage::Format_ARGB32);
    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            pixels[x + y * height()] = static_cast<QRgb>(x+y);
        }
    }
    painter.drawImage(0, 0, image);
    qDebug() << "drawImage time:" << time.elapsed();
    close();
    delete[] pixels;
}

void EmulatorScreenWidget::updateDisplay(uint8_t *display) {
}
