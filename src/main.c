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

    char *filename = argv[1];

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Init failed. error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow(u8"chip8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("window init failed. error: %s", SDL_GetError());
        return -1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("renderer init failed. error: %s", SDL_GetError());
        return -1;
    }
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    // Create texture
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
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    return 0;
                    break;

                // F1 to reload
                case SDLK_F1:
                    init_chip8(&cpu);
                    if (load_rom(&cpu, filename))
                    {
                        printf("reload!!\n");
                    }
                    break;

                // keypadに該当のキーを追加
                default:
                    for (int i = 0; i < 16; i++)
                    {
                        if (event.key.keysym.sym == keyboard[i])
                        {
                            cpu.keypad |= (0x0001 << i);
                            break;
                        }
                    }
                    break;
                }
                break;

            // keypadから該当のキーを削除
            case SDL_KEYUP:
                for (int i = 0; i < 16; i++)
                {
                    if (event.key.keysym.sym == keyboard[i])
                    {
                        cpu.keypad &= ~(0x0001 << i);
                        break;
                    }
                }
            }
        }

        if (cpu.isdraw)
        {
            // 32bitが横に64個、それが縦に32個並んでいる。
            uint32_t pixels[32][64];

            for (int i = 0; i < 32; i++)
            {
                for (int j = 0; j < 64; j++)
                {
                    if (cpu.display[i] & (1ULL << (63 - j)))
                    {
                        pixels[i][j] = 0xFFFFFFFF;
                    }
                    else
                    {
                        pixels[i][j] = 0x000000FF;
                    }
                }
            }

            // texture update
            if (SDL_UpdateTexture(texture, NULL, pixels, sizeof(uint32_t) * 64) != 0)
            {
                printf("texture update failed. error: %s\n", SDL_GetError());
            }
            else
            {
                printf("texture update succeed!!\n");
            }

            // textureをrendererに渡す
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            cpu.isdraw = false;
            printf("success\n");
        }

        SDL_Delay(2);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}