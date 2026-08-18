//6032 3032 6000 7032 4064 6000 120Cでテスト

#include <stdio.h>
#include "../include/chip8.h"

int main(void){
    struct __chip8 cpu;
    init_chip8(&cpu);

    char *filename = "./test_roms/3XNN_4XNN_6XNN_7XNN.ch8";

    if(load_rom(&cpu, filename)){
        printf("Load success!\n");
        while(1){
            emulate_cycle(&cpu);

            printf("V[0]: 0x%02x, PC: 0x%03x", cpu.V[0], cpu.pc);
            getchar(); //enterキー押すまで待機
        }
        
    }else{
        printf("Load failed\n");
        return 1;
    }

    return 0;
}