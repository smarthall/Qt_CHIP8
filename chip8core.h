#ifndef CHIP8CORE_H
#define CHIP8CORE_H

#include "inttypes.h"

class Chip8Core
{
public:
    Chip8Core();
    void reset();
    bool cycle();
    bool load(uint8_t *program, uint16_t size);
    void keyDown(uint8_t key_id);
    void keyUp(uint8_t key_id);
    void copyDisplay(uint8_t *dest);

    // Public Constants
    static const uint16_t LOAD_ADDR = 0x200;
    static const uint16_t MEMORY_SIZE = 4096;
    static const uint16_t MAX_PROGRAM_SIZE = MEMORY_SIZE - LOAD_ADDR;

private:
    // Memory
    uint8_t memory[MEMORY_SIZE];

    // Registers
    static const uint16_t CARRY_REGISTER = 0xF;
    static const uint8_t REGISTER_COUNT = 16;
    uint8_t V[REGISTER_COUNT];

    // Index register
    uint16_t I;

    // Program counter
    uint16_t pc;

    // The screen
    static const uint8_t SCREEN_WIDTH = 64;
    static const uint8_t SCREEN_HEIGHT = 32;
    uint8_t gfx[SCREEN_WIDTH * SCREEN_HEIGHT];
    void clearScreen();

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Stack
    static const uint8_t STACK_SIZE = 16;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;

    // Key inputs
    static const uint8_t KEY_COUNT = 16;
    uint8_t key[KEY_COUNT];
};

#endif // CHIP8CORE_H
