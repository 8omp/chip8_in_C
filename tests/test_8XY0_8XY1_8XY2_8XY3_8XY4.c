//610A 8010 6105 8011 8012 8013 8014 60FF 8014 1212でテスト

#include <stdio.h>
#include "../include/chip8.h"

int main(void){
    struct __chip8 cpu;
    init_chip8(&cpu);

    char *filename = "./test_roms/8XY0_8XY1_8XY2_8XY3_8XY4.ch8";

    if(load_rom(&cpu, filename)){
        printf("Load success!\n");
        while(1){
            emulate_cycle(&cpu);

            printf("V[0]: 0x%02x, V[1]: 0x%02x, V[0xF]: 0x%02x, PC: 0x%03x", cpu.V[0], cpu.V[1], cpu.V[0xF], cpu.pc);
            getchar(); //enterキー押すまで待機
        }
        
    }else{
        printf("Load failed\n");
        return 1;
    }

    return 0;
}