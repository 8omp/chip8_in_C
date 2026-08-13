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

void emulate_cycle(CHIP8 *cpu){

    uint16_t opcode = 0;

    // fetch: 8bitずつ入っているものを、16bitに変換して命令として読めるようにする
    opcode = (uint16_t)cpu->memory[cpu->pc] << 8 | (uint16_t)cpu->memory[cpu->pc + 1];
    cpu->pc += 2;

    // decode & execute
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00FF){
                // 00E0: disp_clear()
                case 0x00E0:
                    break;
                
                // 00EE: return;
                case 0x00EE:
                    break;
                
                // 0NNN: Call
                default:
                
            }
            break;
        
        // 1NNN: goto NNN;
        case 0x1000:
            break;

        // 2NNN: call subroutine at NNN;
        case 0x2000:
            break;

        // 3NNN: if(Vx == NN) then skip the next construction
        case 0x3000:
            break;

        // 4NNN: if(Vx != NN) then skip the next construction
        case 0x4000:
            break;

        // 5NNN: if(Vx == Vy) then skip the next construction
        case 0x5000:
            break;

        // 6NNN: Vx = NN
        case 0x6000:
            break;

        // 7NNN: Vx += NN
        case 0x7000:
            break;

        case 0x8000:
            switch(opcode & 0x000F){
                // 8XY0: Vx = Vy
                case 0x0000:
                    break;
                
                // 8XY1: Vx |= Vy
                case 0x0001:
                    break;

                // 8XY2: Vx &= Vy
                case 0x0002:
                    break;

                // 8XY3: Vx ^= Vy
                case 0x0003:
                    break;

                // 8XY4: Vx += Vy
                case 0x0004:
                    break;

                // 8XY5: Vx -= Vy
                case 0x0005:
                    break;

                // 8XY6: Vx >>= 1
                case 0x0006:
                    break;

                // 8XY7: Vx = Vy - Vx
                case 0x0007:
                    break;

                // 8XYE Vx <<= 1
                case 0x000E:
                    break;
            }
            break;

        // 9XY0: if(Vx != Vy) then skip the next construction
        case 0x9000:
            break;

        // ANNN: I = NNN
        case 0xA000:
            break;

        // BNNN: PC = V0 + NNN
        case 0xB000:
            break;

        // CNNN: Vx = rand() & NN
        case 0xC000:
            break;

        // DXYN: draw(Vx, Vy, N)
        case 0xD000:
            break;

        case 0xE000:
            switch(opcode & 0x00FF){
                // EX9E: if(key() == Vx) then skip the next construction
                case 0x009E:
                    break;

                // EXA1: if(key() != Vx) then skip the next construction
                case 0x00A1:
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF){

                // FX07: Vx = get_delay()
                case 0x0007:
                    break;

                // FX0A: Vx = get_key()
                case 0x000A:
                    break;

                // FX15: delay_timer(Vx)
                case 0x0015:
                    break;

                // FX18: sound_timer(Vx)
                case 0x0018:
                    break;

                // FX1E: I += Vx
                case 0x001E:
                    break;

                // FX29: I = sprite_addr[Vx]
                case 0x0029:
                    break;

                // Fx33: set_BCD(Vx)　*(I+0) = BCD(3);　*(I+1) = BCD(2);　*(I+2) = BCD(1);
                case 0x0033:
                    break;

                // FX55: reg_dump(Vx, &I)
                case 0x0055:
                    break;

                // FX65: reg_load(Vx, &I)
                case 0x0065:
                    break;
            }
            break;
    }

}