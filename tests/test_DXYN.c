#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/chip8.h"

void draw_screen(CHIP8 cpu){
    
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 64; j++){
            if(cpu.display[i] & (1ULL << (63 - j))){
                printf("■");
            }else{
                printf(" ");
            }
        }

        printf("\n");
    }
}

int main(void){

    srand((unsigned int)time(NULL));
    struct __chip8 cpu;
    init_chip8(&cpu);

    char *filename = "../roms/chip8-test-rom.ch8";

    if(load_rom(&cpu, filename)){
        printf("Load success!\n");
        while(1){
            emulate_cycle(&cpu);

            draw_screen(cpu);
            getchar(); //enterキー押すまで待機
        }
        
    }else{
        printf("Load failed\n");
        return 1;
    }

    return 0;
}