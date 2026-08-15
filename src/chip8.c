#include <string.h>
#include <stdio.h>
#include <inttypes.h>

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

    uint16_t nnn = opcode & 0x0FFF;
    uint8_t nn = opcode & 0x00FF;
    uint8_t n = opcode & 0x000F;

    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    // decode & execute
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00FF){
                // 00E0: disp_clear()
                case 0x00E0:
                    memset(cpu->display, 0, sizeof(cpu->display));
                    break;
                
                // 00EE: return;
                case 0x00EE:
                    cpu->pc = cpu->stack[cpu->sp - 1]; 
                    cpu->sp--;
                    break;
                
                // 0NNN: Call
                default:
                //これ要らない
            }
            break;
        
        // 1NNN: goto NNN;
        case 0x1000:
            cpu->pc = nnn;
            break;

        // 2NNN: call subroutine at NNN;
        case 0x2000:
            cpu->stack[cpu->sp] = cpu->pc;
            cpu->sp++;
            break;

        // 3XNN: if(Vx == NN) then skip the next instruction
        case 0x3000:
            if(cpu->V[x] == nn){
                cpu->pc += 2;
            }
            break;

        // 4XNN: if(Vx != NN) then skip the next instruction
        case 0x4000:
            if(cpu->V[x] != nn){
                cpu->pc += 2;
            }
            break;

        // 5XY0: if(Vx == Vy) then skip the next instruction
        case 0x5000:
            if(cpu->V[x] == cpu->V[y]){
                cpu->pc += 2;
            }
            break;

        // 6XNN: Vx = NN
        case 0x6000:
            cpu->V[x] = nn;
            break;

        // 7XNN: Vx += NN
        case 0x7000:
            cpu->V[x] += nn;
            break;

        case 0x8000:
            switch(opcode & 0x000F){
                // 8XY0: Vx = Vy
                case 0x0000:
                    cpu->V[x] = cpu->V[y];
                    break;
                
                // 8XY1: Vx |= Vy
                case 0x0001:
                    cpu->V[x] |= cpu->V[y];
                    break;

                // 8XY2: Vx &= Vy
                case 0x0002:
                    cpu->V[x] &= cpu->V[y];
                    break;

                // 8XY3: Vx ^= Vy
                case 0x0003:
                    cpu->V[x] ^= cpu->V[y];
                    break;

                // 8XY4: Vx += Vy
                case 0x0004:
                    if(cpu->V[x] + cpu->V[y] > 0xFF){
                        cpu->V[0xF] = 1;
                    }else{
                        cpu->V[0xF] = 0;
                    }
                    cpu->V[x] += cpu->V[y];
                    break;

                // 8XY5: Vx -= Vy
                case 0x0005:
                    if(cpu->V[x] >= cpu->V[y]){
                        cpu->V[0xF] = 1;
                    }else{
                        cpu->V[0xF] = 0;
                    }
                    cpu->V[x] -= cpu->V[y];
                    break;

                // 8XY6: Vx >>= 1
                case 0x0006:
                    cpu->V[0xF] = cpu->V[x] & 0x01;
                    cpu->V[x] >>= 1;
                    break;

                // 8XY7: Vx = Vy - Vx
                case 0x0007:
                    if(cpu->V[y] >= cpu->V[x]){
                        cpu->V[0xF] = 1;
                    }else{
                        cpu->V[0xF] = 0;
                    }
                    cpu->V[x] = cpu->V[y] - cpu->V[x];
                    break;

                // 8XYE Vx <<= 1
                case 0x000E:
                    if(cpu->V[x] & 0x80){
                        cpu->V[0xF] = 1;
                    }else{
                        cpu->V[0xF] = 0;
                    }
                    cpu->V[x] <<= 1;
                    break;
            }
            break;

        // 9XY0: if(Vx != Vy) then skip the next instruction
        case 0x9000:
            if(cpu->V[x] != cpu->V[y]){
                cpu->pc += 2;
            }
            break;

        // ANNN: I = NNN
        case 0xA000:
            cpu->I = nnn;
            break;

        // BNNN: PC = V0 + NNN
        case 0xB000:
            cpu->pc = cpu->V[0] + nnn;
            break;

        // CNNN: Vx = rand() & NN
        case 0xC000:
            cpu->V[x] = (rand() & 0xFF) & nn;
            break;

        // DXYN: draw(Vx, Vy, N)
        case 0xD000:

            break;

        case 0xE000:
            switch(opcode & 0x00FF){
                // EX9E: if(key() == Vx) then skip the next instruction
                case 0x009E:
                    //0000 0000 0000 0001を左にV[x]回だけシフトする
                    if(cpu->keypad & (0x0001 << cpu->V[x])){
                        cpu->pc += 2;
                    }
                    break;

                // EXA1: if(key() != Vx) then skip the next instruction
                case 0x00A1:
                    if(!(cpu->keypad & (0x0001 << cpu->V[x]))){
                        cpu->pc += 2;
                    }
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF){

                // FX07: Vx = get_delay()
                case 0x0007:
                    cpu->V[x] = cpu->delay_timer;
                    break;

                // FX0A: Vx = get_key()
                case 0x000A:
                    if(!(cpu->keypad & 0xFFFF)){
                        cpu->pc -= 2;
                    }else{
                        for(int i = 0; i <= 15; i++){
                            if(cpu->keypad & (0x0001 << i)){
                                cpu->V[x] = i;
                                break;
                            }
                        }
                    }
                    break;

                // FX15: delay_timer(Vx)
                case 0x0015:
                    cpu->delay_timer = cpu->V[x];
                    break;

                // FX18: sound_timer(Vx)
                case 0x0018:
                    cpu->sound_timer = cpu->V[x];
                    break;

                // FX1E: I += Vx
                case 0x001E:
                    cpu->I += cpu->V[x];
                    break;

                // FX29: I = sprite_addr[Vx]
                case 0x0029:
                    cpu->I = cpu->V[x] * 0x0005;
                    break;

                // FX33: set_BCD(Vx)　*(I+0) = BCD(3);　*(I+1) = BCD(2);　*(I+2) = BCD(1);
                case 0x0033:
                    cpu->memory[cpu->I] = cpu->V[x] / 100;
                    cpu->memory[cpu->I + 1] = (cpu->V[x] / 10) % 10;
                    cpu->memory[cpu->I + 2] = cpu->V[x] % 10;
                    break;

                // FX55: reg_dump(Vx, &I)
                case 0x0055:
                    for(int i = 0; i <= x; i++){
                        cpu->memory[cpu->I + i] = cpu->V[i];
                    }
                    break;

                // FX65: reg_load(Vx, &I)
                case 0x0065:
                    for(int i = 0; i <= x; i++){
                        cpu->V[i] = cpu->memory[cpu->I + i];
                    }
                    break;
            }
            break;
    }

}