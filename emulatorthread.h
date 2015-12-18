#ifndef EMULATORTHEAD_H
#define EMULATORTHEAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "chip8core.h"

class EmulatorThread : public QThread
{
    Q_OBJECT

public:
    EmulatorThread(Chip8Core *emulator, QObject *parent = 0);
    ~EmulatorThread();

    void start();
    void stop();

    void copyDisplay(uint8_t *dest);

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void displayUpdated();

private:
    static const int speed = 60;

    QMutex local_mutex;
    QMutex running_mutex;
    QWaitCondition condition;

    bool running;
    bool exit;

    Chip8Core *emulator;
};

#endif // EMULATORTHEAD_H
