//2204 1202 00E0 00EEでテスト

#include <stdio.h>
#include "../include/chip8.h"

int main(void){
    struct __chip8 cpu;
    init_chip8(&cpu);

    char *filename = "./test_roms/00E0_00EE_1NNN_2NNN.ch8";

    if(load_rom(&cpu, filename)){
        printf("Load success!\n");
        while(1){
            emulate_cycle(&cpu);

            printf("PC: 0x%04x, SP: 0x%02x, Display[0]: 0x%02x\n", cpu.pc, cpu.sp, cpu.display[0]);
            getchar(); //enterキー押すまで待機
        }
        
    }else{
        printf("Load failed\n");
        return 1;
    }

    return 0;
}