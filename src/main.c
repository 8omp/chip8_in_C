#include <stdio.h>
#include <SDL2/SDL.h>
#include "../include/chip8.h"

#define WIDTH 1024
#define HEIGHT 512
#define TIMERS 1000 / 60

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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        printf("SDL Init failed. error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow(u8"chip8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("window init failed. error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("renderer init failed. error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return -1;
    }
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    // Create texture
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if (texture == NULL)
    {
        printf("renderer init failed. error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyTexture(texture);
        SDL_Quit();
        return -1;
    }

    // Create audio
    SDL_AudioSpec desired;
    memset(&desired, 0, sizeof(SDL_AudioSpec));
    desired.freq = 44100;
    desired.format = AUDIO_S16SYS;
    desired.channels = 1;
    desired.samples = 2048;
    desired.callback = NULL;

    SDL_AudioDeviceID audio = SDL_OpenAudioDevice(NULL, 0, &desired, NULL, 0);
    if (audio == 0)
    {
        printf("audio open failed. error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyTexture(texture);
        SDL_CloseAudioDevice(audio);
        SDL_Quit();
        return -1;
    }

    // Create wav data and add queue
    int16_t data[44100];
    int count = 0;
    for (int i = 0; i < 44100; i++)
    {
        if (count < 50)
        {
            data[i] = 3000;
        }
        else
        {
            data[i] = -3000;
        }

        count++;

        if (count >= 50)
        {
            count = 0;
        }
    }

    if (SDL_QueueAudio(audio, data, sizeof(int16_t) * 44100) != 0)
    {
        printf("audio queue failed. error: %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_CloseAudioDevice(audio);
        SDL_Quit();
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
    int loop_start = SDL_GetTicks();

    // Emulation loop
    while (isrunning)
    {
        emulate_cycle(&cpu);

        if(SDL_GetQueuedAudioSize(audio) < sizeof(data)){
            SDL_QueueAudio(audio, data, sizeof(data));
        }
        if (cpu.sound_timer > 0)
        {
            SDL_PauseAudioDevice(audio, 0);
        }
        else
        {
            SDL_PauseAudioDevice(audio, 1);
        }

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
                        pixels[i][j] = 0xFFFFFFFF; // 白
                    }
                    else
                    {
                        pixels[i][j] = 0x000000FF; // 黒
                    }
                }
            }

            // texture update
            if (SDL_UpdateTexture(texture, NULL, pixels, sizeof(uint32_t) * 64) != 0)
            {
                printf("texture update failed. error: %s\n", SDL_GetError());
            }

            // textureをrendererに渡す
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            cpu.isdraw = false;
        }

        int loop_end = SDL_GetTicks();
        if (loop_end - loop_start >= TIMERS)
        {

            if (cpu.sound_timer > 0)
            {
                cpu.sound_timer--;
            }
            if (cpu.delay_timer > 0)
            {
                cpu.delay_timer--;
            }

            loop_start = SDL_GetTicks();
        }

        SDL_Delay(2);
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audio);
    SDL_Quit();

    return 0;
}