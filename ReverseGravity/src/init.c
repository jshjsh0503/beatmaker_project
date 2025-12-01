/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "defs.h"
#include "init.h"

Mix_Chunk* death_effect = NULL;

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

// ★ renderer 전역 정의
SDL_Renderer* renderer = NULL;

// 이미 존재하는 전역들
SDL_Window* window = NULL;
SDL_Texture* g_tile_textures[3];

// ----------------------------------------
// SDL 라이브러리 및 창 초기화 함수 (InitTiles 로직 통합)
// ----------------------------------------
void InitSDL(void) {
    // 1. 기본 SDL 및 창/렌더러 초기화
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

    // 4. SDL_mixer 초기화
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer 초기화 실패: %s\n", Mix_GetError());
        exit(1);
    }

    // 5. 사운드 로드
    death_effect = Mix_LoadWAV("sound/deathsound.wav");
    if (!death_effect) {
        printf("deathsound.mp3 로드 실패: %s\n", Mix_GetError());
        exit(1);
    }
    printf("Death sound loaded successfully.\n");
}

void QuitSDL(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
void InitTiles(void) {}*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "defs.h"
#include "init.h"

Mix_Chunk* death_effect = NULL;

// 전역 변수 정의
App app;
Entity player;

// 타일 텍스처 전역 정의 (defs.h 에 extern 있음)
SDL_Texture* g_tile_textures[3];

// 폰트 전역 정의 (scene_title.c 에서 extern)
TTF_Font* font_normal = NULL;
TTF_Font* font_selected = NULL;

// ----------------------------------------
// 메모리 초기화
// ----------------------------------------
void InitMemorySet(void) {
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
}

// ----------------------------------------
// PNG 텍스처 로드 함수
// ----------------------------------------
static void load_tile_texture(SDL_Texture** tex_ptr, const char* path) {
    *tex_ptr = IMG_LoadTexture(app.renderer, path);
    if (*tex_ptr == NULL) {
        printf("Error loading texture %s: %s\n", path, IMG_GetError()); 
        exit(1);
    }
    SDL_SetTextureBlendMode(*tex_ptr, SDL_BLENDMODE_BLEND);
}

// ----------------------------------------
// SDL 초기화
// ----------------------------------------
void InitSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL 초기화 실패: %s\n", SDL_GetError());
        exit(1);
    }

    // 창 생성
    app.window = SDL_CreateWindow("Reverse Gravity", 
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!app.window) { printf("창 생성 실패: %s\n", SDL_GetError()); exit(1); }

    // 렌더러 생성
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer) { printf("렌더러 생성 실패: %s\n", SDL_GetError()); exit(1); }

    // 이미지 초기화
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { 
        printf("SDL_image 초기화 실패: %s\n", IMG_GetError()); exit(1); 
    }

    // TTF 폰트 시스템 초기화
    if (TTF_Init() < 0) {
        printf("SDL_ttf 초기화 실패: %s\n", TTF_GetError());
        exit(1);
    }

    // 폰트 로드
    font_normal = TTF_OpenFont("ttf/LiberationSans-Regular.ttf", 32);
    font_selected = TTF_OpenFont("ttf/LiberationSans-Regular.ttf", 40);
    if (!font_normal) {
        printf("폰트 로드 실패: %s\n", TTF_GetError());
        exit(1);
    }
    if (!font_selected) {
        printf("폰트 로드 실패: %s\n", TTF_GetError());
        exit(1);
    }

    // 타일 그래픽 로드
    load_tile_texture(&g_tile_textures[TILE_FLOOR], "./gfx/floor_tile.png");
    load_tile_texture(&g_tile_textures[TILE_SPIKE], "./gfx/spike.png");

    // 사운드 초기화
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer 초기화 실패: %s\n", Mix_GetError());
        exit(1);
    }

    death_effect = Mix_LoadWAV("sound/deathsound.wav");
    if (!death_effect) {
        printf("사운드 로드 실패: %s\n", Mix_GetError());
        exit(1);
    }
}

// ----------------------------------------
// SDL 종료 함수
// ----------------------------------------
void QuitSDL(void)
{
    if (app.renderer) SDL_DestroyRenderer(app.renderer);
    if (app.window) SDL_DestroyWindow(app.window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// ----------------------------------------
// 플레이어 텍스처 로드
// ----------------------------------------
void InitPlayer(void) {
    SDL_Texture* player_tex;

    load_tile_texture(&player_tex, "./gfx/Player.png");
    player.texture = player_tex;

    player.pos.x = SCREEN_WIDTH / 4;
    player.pos.y = SCREEN_HEIGHT / 4;
    player.health = 1;

    SDL_QueryTexture(player.texture, NULL, NULL, &player.pos.w, &player.pos.h);
}

// ----------------------------------------
void InitTiles(void) {}