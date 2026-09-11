#include <stdio.h>
#include <SDL2/SDL.h>
#include "../include/chip8.h"

#define WIDTH 1024
#define HEIGHT 512

uint8_t keyboard[16] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v, // F
};

int main(int argc, char *argv[])
{
    struct __chip8 cpu;
    init_chip8(&cpu);

    char *filename = "../roms/IBM_Logo.ch8";

    // SDLの初期化とwindow, renderer, textureの作成
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Init failed. error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(u8"chip8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("window init failed. error: %s", SDL_GetError());
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("renderer init failed. error: %s", SDL_GetError());
        return -1;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if (texture == NULL)
    {
        printf("renderer init failed. error: %s", SDL_GetError());
        return -1;
    }

    // Load rom
    if (load_rom(&cpu, filename))
    {
        printf("Load success!\n");
    }
    else
    {
        printf("Load failed\n");
        return 1;
    }

    bool isrunning = true;

    // Emulation loop
    while (isrunning)
    {
        emulate_cycle(&cpu);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isrunning = false;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_F1:
                        if(load_rom(&cpu, filename)){
                            printf("reload!!\n");
                        }
                        break;
                }
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}