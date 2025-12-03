// src/defs.h (내용 전체 교체 권장)
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// ----------------------------------------
// 창 및 전역 설정
// ----------------------------------------
#define SCREEN_WIDTH 1600       // 20 tiles * 64 pixels
#define SCREEN_HEIGHT 1280       // 10 tiles * 64 pixels
#define BUFSIZE 1024

// ----------------------------------------
// 맵 및 타일 관련 상수
// ----------------------------------------
#define TILE_SIZE 64            // 타일의 가로/세로 픽셀 크기
#define MAP_WIDTH 25            // 맵의 가로 타일 개수
#define MAP_HEIGHT 20           // 맵의 세로 타일 개수

// 타일 종류 정의 (0: 빈 공간, 1: 바닥, 2: 가시 등)
#define TILE_EMPTY 0          // 빈 공간 (투명)
#define TILE_FLOOR 1          // 일반 바닥/벽 (밟을 수 있음)
#define TILE_SPIKE 2          // 가시 함정 (밟으면 사망)

// ----------------------------------------
// 구조체 정의
// ----------------------------------------

// App: 프로그램 전체 제어를 위한 구조체
typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    TTF_Font *font;
    // 키 상태 변수 (InitMemorySet 함수에서 0으로 초기화됨)
    int key_up;
    int key_down;
    int key_left;
    int key_right;
    int key_r;
    int key_space;
    int key_enter;

    int menu_up;
    int menu_down;
    int menu_enter;
} App;

// Entity: 주인공의 상태를 저장하기 위한 구조체
typedef struct {
    SDL_Texture *texture;
    SDL_Rect pos;           // x, y, w, h 좌표 및 크기
    double v_x;             // x방향 속도 (실수형으로 사용)
    double v_y;             // y방향 속도
    int health;             // 생존 상태 (1: 생존, 0: 사망)
    
    // 플랫폼 게임 로직을 위한 추가 변수
    int is_grounded;        // 땅에 붙어있는지 (점프 가능 여부)
    int gravity_inverted;   // 중력 역전 상태 (0: 일반, 1: 역전)
} Entity;

// Text: 점수판 등 문자열 표시를 위한 구조체 (현재 프로젝트에서는 사용하지 않음)
typedef struct {
    SDL_Rect pos;
    SDL_Color color;
    SDL_Surface *surface;
    SDL_Texture *texture;
} Text;

// ----------------------------------------
// 전역 변수 선언 (extern)
// ----------------------------------------

extern App app;
extern Entity player;
extern Mix_Music *bgm;
extern Mix_Chunk *death_effect;

// 맵 관련 전역 변수
extern int g_map_data[MAP_HEIGHT][MAP_WIDTH];
extern SDL_Texture* g_tile_textures[3]; // TILE_EMPTY, TILE_FLOOR, TILE_SPIKE 텍스처 저장

// ----------------------------------------
// 링커 오류 방지를 위해 임시로 Mix_Chunk, TTF_Font 포인터 선언
// 이 부분을 지우면 컴파일 오류가 날 수 있습니다.
// ----------------------------------------

typedef enum {
    STATE_TITLE,
    STATE_GAME,
    STATE_GAMEOVER,
    STATE_EXIT
} GameState;

extern GameState game_state;

void ResetInputFlags(void);