#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"
#include "init.h"

// ----------------------------------------
// 전역 변수 초기화 함수
// ----------------------------------------
void InitMemorySet(void) {
    // App 구조체와 Player 구조체 메모리를 0으로 초기화
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
}

// ----------------------------------------
// 타일 맵 텍스처 로드 헬퍼 함수
// ----------------------------------------
static void load_tile_texture(SDL_Texture** tex_ptr, const char* path) {
    *tex_ptr = IMG_LoadTexture(app.renderer, path);
    if (*tex_ptr == NULL) {
        printf("Error loading texture %s: %s\n", path, IMG_GetError()); 
        exit(1); // 로드 실패 시 강제 종료 (디버깅 용이)
    }
    // 투명도 블렌딩 모드 설정 (PNG 투명도를 인식하게 만드는 핵심)
    SDL_SetTextureBlendMode(*tex_ptr, SDL_BLENDMODE_BLEND);
}


// ----------------------------------------
// SDL 라이브러리 및 창 초기화 함수 (InitTiles 로직 통합)
// ----------------------------------------
void InitSDL(void) {
    // 1. 기본 SDL 및 창/렌더러 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 초기화 실패: %s\n", SDL_GetError());
        exit(1);
    }
    app.window = SDL_CreateWindow("Reverse Gravity", 
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!app.window) { printf("창 생성 실패: %s\n", SDL_GetError()); exit(1); }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer) { printf("렌더러 생성 실패: %s\n", SDL_GetError()); exit(1); }
    
    // 2. SDL_image 초기화 (PNG 지원)
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { 
        printf("SDL_image 초기화 실패: %s\n", IMG_GetError()); exit(1); 
    }

    // 3. 타일 텍스처 로드 (인라인 함수 사용)
    load_tile_texture(&g_tile_textures[TILE_FLOOR], "./gfx/floor_tile.png");
    load_tile_texture(&g_tile_textures[TILE_SPIKE], "./gfx/spike.png");
    
    printf("Tiles loaded successfully.\n");
}

// ----------------------------------------
// 플레이어 초기화 함수
// ----------------------------------------
void InitPlayer(void) {
    SDL_Texture* player_tex;
    
    // load_tile_texture 함수를 재활용하여 로드 및 투명도 설정
    load_tile_texture(&player_tex, "./gfx/Player.png"); 
    player.texture = player_tex;
    
    player.pos.x = SCREEN_WIDTH / 4;
    player.pos.y = SCREEN_HEIGHT / 4;
    player.health = 1;
    
    // 텍스처 크기 설정
    SDL_QueryTexture(player.texture, NULL, NULL, &player.pos.w, &player.pos.h);
}

// ----------------------------------------
// InitTiles 함수는 InitSDL에 통합되었으므로,
// 링커 오류 방지용 빈 함수로 정의합니다.
// ----------------------------------------
void InitTiles(void) {}