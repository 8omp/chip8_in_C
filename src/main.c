#include <stdio.h>
#include "../include/chip8.h"

int main(int argc, char *[]){
    struct __chip8 *cpu;
    init_chip8(cpu);

    char *filename = "../roms/IBM_Logo.ch8";

    if(load_rom(cpu, filename)){
        printf("Load success!\n");

        for(int i=0; i<10; i++){
            printf("%02X\n", cpu->memory[0x200 + i]);
        }
    }

    return 0;
}