#include "emulatorthread.h"

#include <QDebug>

EmulatorThread::EmulatorThread(Chip8Core *emulator, QObject *parent)
    : QThread(parent)
{
    running = false;
    exit = false;

    this->emulator = emulator;
}

EmulatorThread::~EmulatorThread() {
    // Tell thread to exit
    local_mutex.lock();
    exit = true;
    condition.wakeOne();
    local_mutex.unlock();

    // Wait for it to exit
    wait();
}

void EmulatorThread::start() {
    QMutexLocker locker(&local_mutex);

    // Start thread if we arent running
    if (!isRunning()) QThread::start();

    // Tell thread to start
    running = true;
    condition.wakeOne();
}

void EmulatorThread::stop() {
    QMutexLocker locker(&local_mutex);

    // Tell thread to stop cycling the emulator
    running = false;

    // Block until the emulator stops
    running_mutex.lock();
    running_mutex.unlock();
}

void EmulatorThread::copyDisplay(uint8_t *dest) {
    QMutexLocker locker(&running_mutex);

    // Copy display from paused emulator
    emulator->copyDisplay(dest);
}

void EmulatorThread::run() {
    forever {
        // Check what we are doing
        local_mutex.lock();
        if (exit) return;
        bool running = this->running;
        local_mutex.unlock();

        if (running) {
            // Running
            running_mutex.lock();
            bool display_update = emulator->cycle();
            running_mutex.unlock();

            // Update anyone who cares about the screen
            if (display_update) {
                emit displayUpdated();
            }

            // Wait for the next cycle
            local_mutex.lock();
            condition.wait(&local_mutex, 1000UL/speed);
            local_mutex.unlock();
        } else {
            // Not running

            // Wait till someone restarts us
            local_mutex.lock();
            condition.wait(&local_mutex);
            local_mutex.unlock();
        }
    }
}
