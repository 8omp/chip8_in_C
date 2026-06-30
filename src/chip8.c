#include <string.h>
#include <stdio.h>

#include "../include/chip8.h"

uint8_t fonts[80] = {
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

void init_chip8(CHIP8 *cpu){
    
    // cpu->pc = 0x200;
    // cpu->I = 0;
    // cpu->sp = 0;
    // cpu->keypad = 0;

    // cpu->delay_timer = 60;
    // cpu->sound_timer = 60;

    // for(int i = 0; i < 16; i++){
    //     cpu->general_register[i] = 0;
    //     cpu->stack[i] = 0;
    // }

    // for(int i = 0; i < 32; i++){
    //     cpu->display[i] = 0;
    // }

    // for(int i = 0; i < 80; i++){
    //     cpu->memory[i] = fonts[i];
    // }

    // これで構造体の全てが0で初期化できる
    memset(cpu, 0, sizeof(*cpu));

    cpu->pc = 0x200;

    for(int i = 0; i < 80; i++){
        cpu->memory[i] = fonts[i];
    }

}

bool load_rom(CHIP8 *cpu, const char *filename){

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        printf("File open failed.\n");
        return false;
    }

    //カーソルを末尾に移動
    fseek(fp, 0, SEEK_END);

    long rom_size = ftell(fp);

    //カーソルを先頭に移動
    fseek(fp, 0, SEEK_SET);

    if(rom_size < sizeof(cpu -> memory) - 0x200){
        size_t read_count = fread(cpu->memory+0x200, sizeof(uint8_t), rom_size, fp);
    }else{
        fclose(fp);
        return false;
    }

    fclose(fp);

    return true;

}