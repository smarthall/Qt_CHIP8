#include "chip8core.h"
#include "string.h"
#include <QFile>

Chip8Core::Chip8Core()
{

}

void Chip8Core::reset() {
    // Fonts
    static const uint8_t fontset[] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Init core registers
    pc = LOAD_ADDR;
    I = 0;

    // Clear Screen
    clearScreen();

    // Clear Stack
    sp = 0;
    memset(stack, 0, sizeof(stack));

    // Clear Registers
    memset(V, 0, sizeof(V));

    // Clear Memory
    memset(memory, 0, sizeof(memory));

    // Load Fonts
    memcpy(memory, fontset, sizeof(fontset));

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;
}

void Chip8Core::clearScreen() {
    memset(gfx, 0, sizeof(gfx));
}

int Chip8Core::load(char *filename) {
    QFile file(filename);

    // Open the file as binary and read only
    if (!file.open(QIODevice::ReadOnly))
        return 0;

    // TODO
    return 1;
}
