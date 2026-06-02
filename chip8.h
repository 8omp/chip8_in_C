#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define MEMORY_SIZE 4096
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct{

    uint8_t memory[MEMORY_SIZE]; 

    /// 汎用レジスタ V0〜VF
    uint8_t V[REGISTER_COUNT];

    // インデックスレジスタ
    uint16_t I;

    // プログラムカウンタ
    uint16_t PC;

    // スタックポインタ
    uint8_t SP;

    uint16_t stack[STACK_SIZE];

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t display[DISPLAY_WIDTH * DISPLAY_HEIGHT];

    bool keypad[16];

}CHIP8;

void chip8_init(CHIP8 *chip8);
void chip8_load_rom(CHIP8 *chip8, const char *filename);
void chip8_reset(CHIP8 *chip8);

#endif // CHIP8_H