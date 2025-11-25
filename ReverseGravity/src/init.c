#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // memset 사용을 위해 필수
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"
#include "init.h"

// ----------------------------------------
// 전역 변수 초기화 함수 (링커 오류 해결의 핵심)
// ----------------------------------------
void InitMemorySet(void) {
    // App 구조체와 Player 구조체 메모리를 0으로 초기화
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
}

// ----------------------------------------
// 타일 맵 텍스처 로드 함수
// ----------------------------------------
void InitTiles(void) {
    // 바닥 타일 이미지 로드
    SDL_Texture* floor_tex = IMG_LoadTexture(app.renderer, "./gfx/floor_tile.png");

    if (floor_tex == NULL) {
        printf("Error loading floor tile: %s\n", IMG_GetError());
        exit(1);
    }

    // 바닥(1)과 가시(2) 모두 같은 텍스처 사용 (임시)
    g_tile_textures[TILE_FLOOR] = floor_tex;
    g_tile_textures[TILE_SPIKE] = floor_tex;
    
    printf("Tiles loaded successfully.\n");
}

// ----------------------------------------
// SDL 라이브러리 및 창 초기화 함수
// ----------------------------------------
void InitSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 초기화 실패: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Reverse Gravity", 
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    if (!app.window) {
        printf("창 생성 실패: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

    if (!app.renderer) {
        printf("렌더러 생성 실패: %s\n", SDL_GetError());
        exit(1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image 초기화 실패: %s\n", IMG_GetError());
        exit(1);
    }

    // 타일 초기화 호출
    InitTiles();
}

// ----------------------------------------
// 플레이어 초기화 함수
// ----------------------------------------
void InitPlayer(void) {
    player.texture = IMG_LoadTexture(app.renderer, "./gfx/Player.png");
    
    // 플레이어 이미지가 없어도 실행은 되도록 예외 처리
    if (player.texture == NULL) {
        printf("Warning: Player texture not found. (%s)\n", IMG_GetError());
    }

    player.pos.x = SCREEN_WIDTH / 4;
    player.pos.y = SCREEN_HEIGHT / 4;
    player.health = 1;
    
    // 텍스처가 로드되었다면 크기 설정
    if (player.texture) {
        SDL_QueryTexture(player.texture, NULL, NULL, &player.pos.w, &player.pos.h);
    } else {
        player.pos.w = 64; // 기본 크기
        player.pos.h = 64;
    }
}
