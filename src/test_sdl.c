#include <SDL2/SDL.h>
#include <stdbool.h>

// ウィンドウのサイズ
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 512;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

bool is_running = true;

int main(int argc, char* argv[]){

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("SDLの初期化に失敗。error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow(u8"SDL2テスト", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("ウィンドウの初期化に失敗。error: %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL){
        printf("ウィンドウの初期化に失敗。error: %s", SDL_GetError());
        return -1;
    }

    while(is_running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                is_running = false;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
