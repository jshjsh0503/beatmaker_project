#include<SDL2/SDL.h>

int main(void){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        800,600,
        SDL_WINDOW_SHOWN
    );
    if(!win){
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    //메인 루프 : 창을 닫을 때 까지 이벤트 처리
    int running = 1;
    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                running = 0;
            }
        }
        SDL_Delay(16);
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}