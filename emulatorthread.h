#ifndef EMULATORTHEAD_H
#define EMULATORTHEAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>

#include "chip8core.h"

class EmulatorThread : public QThread
{
    Q_OBJECT

public:
    EmulatorThread(QObject *parent = 0);
    ~EmulatorThread();

    void start();
    void stop();

    void copyDisplay(uint8_t *dest);
    void load(QFile &file);
protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void displayUpdated();
    void playSound();

public slots:
    void keyUp(uint8_t key_id);
    void keyDown(uint8_t key_id);

private:
    static const int speed = 120;

    QMutex local_mutex;
    QMutex running_mutex;
    QWaitCondition condition;

    bool running;
    bool exit;

    Chip8Core emulator;
};

#endif // EMULATORTHEAD_H
