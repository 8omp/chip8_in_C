#include <stdio.h>
#include "../include/chip8.h"

int main(int argc, char *argv[]){
    struct __chip8 cpu;
    init_chip8(&cpu);

    char *filename = "../roms/IBM_Logo.ch8";

    if(load_rom(&cpu, filename)){
        printf("Load success!\n");
        while(1){
            emulate_cycle(&cpu);
        }
        
    }else{
        printf("Load failed\n");
        return 1;
    }

    return 0;
}