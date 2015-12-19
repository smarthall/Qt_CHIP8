#include "emulatorthread.h"

#include <QDebug>

EmulatorThread::EmulatorThread(QObject *parent)
    : QThread(parent)
{
    running = false;
    exit = false;
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
    emulator.copyDisplay(dest);
}

void EmulatorThread::load(QFile &rom) {
    QMutexLocker locker(&running_mutex);

    if (!rom.open(QIODevice::ReadOnly))
        return;

    QByteArray program = rom.read(emulator.MAX_PROGRAM_SIZE);

    emulator.load((uint8_t*)program.constData(), program.size());
    emit displayUpdated();
}

void EmulatorThread::keyUp(uint8_t key_id)
{
    QMutexLocker locker(&running_mutex);

    emulator.keyUp(key_id);
}

void EmulatorThread::keyDown(uint8_t key_id)
{
    QMutexLocker locker(&running_mutex);

    emulator.keyDown(key_id);
}

void EmulatorThread::run() {
    forever {
        // Check what we are doing
        local_mutex.lock();
        if (exit) {
            local_mutex.unlock();
            return;
        }
        bool running = this->running;
        local_mutex.unlock();

        if (running) {
            // Running
            running_mutex.lock();
            emulator.cycle();
            bool display_update = emulator.getDisplayDirty();
            bool play_sound = emulator.getPlaySound();
            running_mutex.unlock();

            // Update anyone who cares about the screen
            if (display_update) {
                emit displayUpdated();
            }

            if (play_sound) {
                emit playSound();
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
