#include "emulatorthread.h"

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
    if (!isRunning()) QThread::start();

    // Tell thread to start
    local_mutex.lock();
    running = true;
    condition.wakeOne();
    local_mutex.unlock();
}

void EmulatorThread::stop() {
    // Tell thread to stop cycling the emulator
    local_mutex.lock();
    running = false;
    local_mutex.unlock();

    // Block until the emulator stops
    running_mutex.lock();
    running_mutex.unlock();
}

void EmulatorThread::copyDisplay(uint8_t *dest) {
    // Copy display from paused emulator
    running_mutex.lock();
    emulator->copyDisplay(dest);
    running_mutex.unlock();
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
            condition.wait(&local_mutex, 1000UL/speed);
        } else {
            // Not running

            // Wait till someone restarts us
            condition.wait(&local_mutex);
        }
    }
}
