#include <SDL2/SDL.h>
#include <stdbool.h>

// ウィンドウのサイズ
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 512;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

int main(int argc, char* argv[]){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDL Init failed. error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow(u8"SDL2テスト", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("window init failed. error: %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        printf("renderer init failed. error: %s", SDL_GetError());
        return -1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if(texture == NULL){
        printf("renderer init failed. error: %s", SDL_GetError());
        return -1;
    }

    bool isrunning = true;

    while(isrunning){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                isrunning = false;
            }
        }

        //これで背景を決定してclear
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //これで描く
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 0, 512, 256);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
