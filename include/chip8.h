#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
typedef struct  __chip8{

    uint8_t memory[4096];
    uint16_t I; //index register
    uint8_t general_register[16];//V0 ~ VF, VFはフラグレジスタ
    uint16_t pc;

    uint8_t sp; //stack pointer
    uint16_t stack[16];

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint64_t display[32]; //64bit * 32 = 256byte

    uint16_t keypad; //keypad (0 ~ 9, A ~ F)

}CHIP8;

#endif // CHIP8_H