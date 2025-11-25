// src/input.c
#include <SDL2/SDL.h>
#include <stdlib.h> // exit() 사용
#include "defs.h"

// SDL_QUIT 이벤트를 처리하고 프로그램을 종료합니다.
static void QuitSDL(void) {
    // SDL 자원 해제 코드가 이 함수 안에 구현되어 있을 것입니다.
    // 임시로 exit(0) 처리
    exit(0);
}

// 키보드 키를 뗐을 때 (KeyUp)의 반응을 처리합니다.
void ResponseKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) {
        SDL_Scancode key_input = event->keysym.scancode;
        switch (key_input) {
            case SDL_SCANCODE_UP:
                app.key_up = 0;
                break;
            case SDL_SCANCODE_DOWN:
                app.key_down = 0;
                break;
            case SDL_SCANCODE_LEFT:
                app.key_left = 0;
                break;
            case SDL_SCANCODE_RIGHT:
                app.key_right = 0;
                break;
            case SDL_SCANCODE_R:
                app.key_r = 0;
                break;
            // 다른 키가 있다면 여기에 추가
            default:
                break;
        }
    }
}

// 키보드 키를 눌렀을 때 (KeyDown)의 반응을 처리합니다.
void ResponseKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0) { // 키를 길게 눌러 반복되는 이벤트는 무시
        SDL_Scancode key_input = event->keysym.scancode;
        switch (key_input) {
            case SDL_SCANCODE_UP:
                app.key_up = 1;
                break;
            case SDL_SCANCODE_DOWN:
                app.key_down = 1;
                break;
            case SDL_SCANCODE_LEFT:
                app.key_left = 1;
                break;
            case SDL_SCANCODE_RIGHT:
                app.key_right = 1;
                break;
            case SDL_SCANCODE_R: // 중력 역전 기믹에 사용할 R 키
                app.key_r = 1;
                break;
            // ESC 키를 눌러도 종료되도록 처리 (옵션)
            case SDL_SCANCODE_ESCAPE:
                QuitSDL();
                break;
            default:
                break;
        }
    }
}

// SDL 이벤트 폴링을 통해 입력을 처리하는 메인 함수
void GetInput(void) {
    SDL_Event event;

    // 대기 중인 모든 이벤트를 처리합니다.
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: // 창 닫기 버튼
                QuitSDL();
                break;
            case SDL_KEYDOWN: // 키 누름 이벤트
                ResponseKeyDown(&event.key);
                break;
            case SDL_KEYUP: // 키 뗌 이벤트
                ResponseKeyUp(&event.key);
                break;
            default:
                break;
        }
    }
}
