#include "chip8core.h"

#include <string.h>
#include <cstdlib>
#include <iostream>

Chip8Core::Chip8Core()
{
    reset();
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

    // Unpress all keys
    memset(key, 0, sizeof(key));

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;
}

void Chip8Core::clearScreen() {
    memset(gfx, 0, sizeof(gfx));
}

bool Chip8Core::load(uint8_t *program, uint16_t size) {
    // No point loading a new program into a running CPU
    reset();

    // Check if the program fits
    if (size > (MEMORY_SIZE - LOAD_ADDR)) return false;

    // Copy in the program
    memcpy(memory + LOAD_ADDR, program, sizeof(uint8_t) * size);

    return true;
}

void Chip8Core::keyDown(uint8_t key_id) {
    key[key_id] = 1;
}

void Chip8Core::keyUp(uint8_t key_id) {
    key[key_id] = 0;
}

void Chip8Core::copyDisplay(uint8_t *dest) {
    memcpy(dest, gfx, sizeof(gfx));
}

bool Chip8Core::cycle() {
    bool gfx_update = false;

    uint8_t x, y;
    uint16_t n, opcode;
    bool pressed;
    int res;

    opcode = (memory[pc] << 8 | memory[pc + 1]);

    switch (opcode & 0xF000) {
    // 0XXX: Misc opcodes
    case 0x0000:
        switch (opcode) {
        // 00E0: Clears the screen.
        case 0x00E0:
            clearScreen();
            pc += 2;
            break;

            // 00EE: Returns from a subroutine.
        case 0x00EE:
            pc = stack[--sp] + 2;
            break;

            // 0NNN: Calls RCA 1802 program at address NNN. Not necessary for most ROMs.
        default:
            std::cout << "Unknown opcode:" << std::hex << opcode;
        }
        break;

        // 1NNN: Jumps to address NNN.
    case 0x1000:
        pc = opcode & 0x0FFF;
        break;

        // 2NNN: Calls subroutine at NNN.
    case 0x2000:
        stack[sp++] = pc;
        pc = opcode & 0x0FFF;
        break;

        // 3XNN: Skips the next instruction if VX equals NN.
    case 0x3000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;
        if (V[x] == n) {
            pc += 4;
        } else {
            pc += 2;
        }
        break;

        // 4XNN: Skips the next instruction if VX doesn't equal NN.
    case 0x4000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;
        if (V[x] != n) {
            pc += 4;
        } else {
            pc += 2;
        }
        break;

        // 5XY0: Skips the next instruction if VX equals VY.
    case 0x5000:
        x = (opcode & 0x0F00) >> 8;
        y = (opcode & 0x00F0) >> 4;
        if (V[x] == V[y]) {
            pc += 4;
        } else {
            pc += 2;
        }
        break;

        // 6XNN: Sets VX to NN.
    case 0x6000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;
        V[x] = n;
        pc += 2;
        break;

        // 7XNN: Adds NN to VX.
    case 0x7000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;
        res = V[x] + n;

        if (res > 0xFF) {
            V[CARRY_REGISTER] = 1;
        } else {
            V[CARRY_REGISTER] = 0;
        }
        V[x] = V[x] + n;
        pc += 2;
        break;

        // 8XXX: Various Register operations
    case 0x8000:
        // Get the two registers
        x = (opcode & 0x0F00) >> 8;
        y = (opcode & 0x00F0) >> 4;

        switch (opcode & 0x000F)
        {
        // 8XY0: Sets VX to the value of VY.
        case 0x0:
            V[x] = V[y];
            pc += 2;
            break;

            // 8XY1: Sets VX to VX or VY.
        case 0x1:
            V[x] = V[x] | V[y];
            pc += 2;
            break;

            // 8XY2: Sets VX to VX and VY.
        case 0x2:
            V[x] = V[x] & V[y];
            pc += 2;
            break;

            // 8XY3: Sets VX to VX xor VY.
        case 0x3:
            V[x] = V[x] ^ V[y];
            pc += 2;
            break;

            // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
        case 0x4:
            res = V[x] + V[y];
            if (res > 0xFF) {
                V[CARRY_REGISTER] = 1;
            } else {
                V[CARRY_REGISTER] = 0;
            }
            V[x] = V[x] + V[y];
            pc += 2;
            break;

            // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
        case 0x5:
            if (V[x] < V[y]) {
                V[CARRY_REGISTER] = 0;
            } else {
                V[CARRY_REGISTER] = 1;
            }
            V[x] = V[x] - V[y];
            pc += 2;
            break;

            // 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
        case 0x6:
            V[CARRY_REGISTER] = V[x] & 0x01;
            V[x] = V[x] >> 1;
            pc += 2;
            break;

            // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
        case 0x7:
            if (V[y] < V[x]) {
                V[CARRY_REGISTER] = 0;
            } else {
                V[CARRY_REGISTER] = 1;
            }
            V[x] = V[y] - V[x];
            pc += 2;
            break;

            // 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
        case 0xE:
            V[CARRY_REGISTER] = (V[x] & 0x80) >> 7;
            V[x] = V[x] << 1;
            pc += 2;
            break;

        default:
            std::cout << "Unknown opcode:" << std::hex << opcode;
            break;
        }
        break;

        // 9XY0: Skips the next instruction if VX doesn't equal VY.
    case 0x9000:
        x = (opcode & 0x0F00) >> 8;
        y = (opcode & 0x00F0) >> 4;
        if (V[x] == V[y]) {
            pc += 4;
        } else {
            pc += 2;
        }
        break;

        // ANNN: Sets I to the address NNN.
    case 0xA000:
        I = opcode & 0x0FFF;
        pc += 2;
        break;

        // BNNN: Jumps to the address NNN plus V0.
    case 0xB000:
        pc = (opcode & 0x0FFF) + V[0];
        break;

        // CXNN: Sets VX to the result of a bitwise and operation on a random number and NN.
    case 0xC000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;
        V[x] = std::rand() & n;
        pc += 2;
        break;

        // DXYN: Sprites stored in memory at location in index register (I), 8bits wide.
        //       Wraps around the screen.
        //       If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero.
        //       All drawing is XOR drawing (i.e. it toggles the screen pixels).
        //       Sprites are drawn starting at position VX, VY.
        //       N is the number of 8bit rows that need to be drawn.
        //       If N is greater than 1, second line continues at position VX, VY+1, and so on.
    case 0xD000:
        x = (opcode & 0x0F00) >> 8;
        y = (opcode & 0x00F0) >> 4;
        n = opcode & 0x000F;

        uint8_t pixel;

        V[CARRY_REGISTER] = 0;
        // For the number of lines
        for (int yline = 0; yline < n; yline++)
        {
            // Read the pixel data
            pixel = memory[I + yline];
            for(int xline = 0; xline < 8; xline++)
            {
                // Move 0b1000 0000 across for each column
                if((pixel & (0x80 >> xline)) != 0)
                {
                    uint8_t new_x = (V[x] + xline) % SCREEN_WIDTH;
                    uint8_t new_y = (V[y] + yline) % SCREEN_HEIGHT;
                    uint16_t index = (new_x + (new_y * SCREEN_WIDTH));
                    if(gfx[index] == 1) {
                        V[CARRY_REGISTER] = 1;
                        gfx[index] = 0;
                    } else {
                        gfx[index] = 1;
                    }
                }
            }
        }

        gfx_update = true;
        pc += 2;

        break;

        // EXXX: Some key dependent routines
    case 0XE000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;

        switch (n) {
        // EX9E: Skips the next instruction if the key stored in VX is pressed.
        case 0x9E:
            if (key[V[x]] != 0) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

            // EXA1: Skips the next instruction if the key stored in VX isn't pressed.
        case 0xA1:
            if (key[V[x]] == 0) {
                pc += 4;
            } else {
                pc += 2;
            }
            break;

        default:
            std::cout << "Unknown opcode:" << std::hex << opcode;
            break;
        }
        break;

        // FXXX: IO Routines
    case 0xF000:
        x = (opcode & 0x0F00) >> 8;
        n = opcode & 0x00FF;

        switch (n) {
        // FX07: Sets VX to the value of the delay timer.
        case 0x07:
            V[x] = delay_timer;
            pc += 2;
            break;

            // FX0A: A key press is awaited, and then stored in VX.
        case 0x0A:
            pressed = false;

            for (uint8_t i = 0; i < KEY_COUNT; i++) {
                if (key[i] != 0) {
                    pressed = true;
                    V[x] = i;
                    break;
                }
            }

            if (pressed) {
                pc += 2;
            } else {
                // Dont advance counter/timers until key is pressed.
                return false;
            }

            break;

            // FX15: Sets the delay timer to VX.
        case 0x15:
            delay_timer = V[x];
            pc += 2;
            break;

            // FX18: Sets the sound timer to VX.
        case 0x18:
            sound_timer = V[x];
            pc += 2;
            break;

            // FX1E: Adds VX to I.
        case 0x1E:
            if (I + V[x] > 0xFFF) {
                V[CARRY_REGISTER] = 1;
            } else {
                V[CARRY_REGISTER] = 0;
            }
            I = I + V[x];
            pc += 2;
            break;

            // FX29: Sets I to the location of the sprite for the character in VX.
            //       Characters 0-F (in hexadecimal) are represented by a 4x5 font.
        case 0x29:
            I = V[x] * 5;
            pc += 2;
            break;

            // FX33: Stores the Binary-coded decimal representation of VX,
            //       with the most significant of three digits at the address in I,
            //       the middle digit at I plus 1, and the least significant digit at I plus 2.
        case 0x33:
            memory[I]     = V[x] / 100;
            memory[I + 1] = V[x] % 100 / 10;
            memory[I + 2] = V[x] % 10;
            pc += 2;
            break;

            // FX55: Stores V0 to VX in memory starting at address I.
        case 0x55:
            for (int i = 0; i <= x; i++) {
                memory[I + i] = V[i];
            }
            pc += 2;
            break;

            // FX65: Fills V0 to VX with values from memory starting at address I.
        case 0x65:
            for (int i = 0; i <= x; i++) {
                V[i] = memory[I + i];
            }
            pc += 2;
            break;

        default:
            std::cout << "Unknown opcode:" << std::hex << opcode;
            break;
        }
        break;


        // Unknown Opcode
    default:
        std::cout << "Unknown opcode:" << std::hex << opcode;
        break;
    }

    // Update timers
    if (delay_timer > 0) delay_timer--;
    if (sound_timer > 0) {
        if (sound_timer == 1) std::cout << '\a';
        sound_timer--;
    }

    return gfx_update;
}
