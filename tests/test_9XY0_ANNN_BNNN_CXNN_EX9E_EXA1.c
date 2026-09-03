//6001 6101 9010 ABCD B20B 60EE 60FF C001 E09E 60FF E0A1 1216でテスト
#include <stdio.h>
#include <time.h>
#include "../include/chip8.h"

int main(void){

    srand((unsigned int)time(NULL));
    struct __chip8 cpu;
    init_chip8(&cpu);

    cpu.keypad = 0x0002;

    char *filename = "./test_roms/9XY0_ANNN_BNNN_CXNN_EX9E_EXA1.ch8";

    if(load_rom(&cpu, filename)){
        printf("Load success!\n");
        while(1){
            emulate_cycle(&cpu);

            printf("V[0]: 0x%02x, V[1]: 0x%02x, keypad: 0x%04x, PC: 0x%03x, I: 0x%03x", cpu.V[0], cpu.V[1], cpu.keypad, cpu.pc, cpu.I);
            getchar(); //enterキー押すまで待機
        }
        
    }else{
        printf("Load failed\n");
        return 1;
    }

    return 0;
}