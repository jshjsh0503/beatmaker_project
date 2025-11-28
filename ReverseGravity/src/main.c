#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // InitMemorySet을 위해 필요 (init.c에 정의되어 있지만, 안전을 위해 포함)

#include "defs.h"    // <-- 1. 프로젝트의 모든 상수 및 구조체 정의
#include "init.h"    // <-- 2. 초기화 함수 선언 (InitMemorySet 포함)
#include "draw.h"
#include "action.h"
#include "input.h"


// ----------------------------------------
// 전역 변수 정의
// ----------------------------------------
App app;
Entity player;
Mix_Music *bgm;
extern Mix_Chunk *death_effect;

// 타일 텍스처 배열 정의 (defs.h에서 extern으로 선언됨)
SDL_Texture* g_tile_textures[3] = {NULL};

// 맵 데이터 정의 (defs.h에서 extern으로 선언됨)
// 0: 빈 공간, 1: 바닥, 2: 가시
int g_map_data[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    // 1행: 천장에 매달린 가시 (중력 역전 테스트용)
    {1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    // 4행: 중앙에 떠 있는 벽돌 플랫폼
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
    // 8행: 바닥에 설치된 가시 (일반 중력 테스트용)
    {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    // 9행: 바닥 (모두 벽돌)
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    // ----------------------------------------------------
};


// ----------------------------------------
// 메인 함수
// ----------------------------------------
int main(void) {
    // 1. 초기화 (이 함수를 찾지 못하고 있습니다.)
    InitMemorySet(); 
    InitSDL();
    InitPlayer();   
    
    // 2. 프로그램 무한 루프 (메인 게임 루프)
    for (;;) {
        ClearWindow();
        GetInput();

        // 게임 진행 로직
        if (player.health) { 
            ActGame();
            DrawGame();
        } else {
            ActGameOver();
            DrawGameOver();
        }

        ShowWindow();
        SDL_Delay(16);
    }
    
    return 0;
}


// ----------------------------------------
// 링커 오류 방지를 위한 더미 함수 정의 (main.c가 호출하지만 로직이 없는 함수들)
// ----------------------------------------
void InitGameOver(void) {}
void InitBullet(void) {}
void InitScoreBoard(void) {}
void InitSound(void) {}
void LoadSound(void) {}
void PlayBGM(void) {}
void InitTTF(void) {}
