#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "defs.h"
#include "init.h"
#include "draw.h"
#include "action.h"
#include "input.h"
#include "scene_title.h"

Mix_Music *bgm;
extern Mix_Chunk *death_effect;

// ★ 맵 데이터 수정 (천장에 3번 타일 배치)
int g_map_data[MAP_HEIGHT][MAP_WIDTH] =
{
    // Row 0: 천장
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    // Row 1: 천장 바로 아래
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    
    // Row 2: ★ 여기에 3번(거꾸로 가시) 배치 테스트!
    {1,0,0,0,0,0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,1},

    // Row 3~19 (나머지는 0 또는 기존 맵 유지)
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0}, // Goal
    {1,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,1,0,0,0,0,1}, // 바닥 가시
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1}, // Start
    {1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1}, // 바닥
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

GameState game_state = STATE_TITLE;

int main(void) {
    InitMemorySet(); 
    InitSDL();
    InitPlayer();
    title_init();    
    
    for (;;) {
        ClearWindow();

        if (game_state == STATE_ENDING) 
        {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) QuitSDL();
                if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) QuitSDL();
            }
        }
        else GetInput();

        switch (game_state) 
        {
            case STATE_TITLE: title_update(); title_render(); break;
            case STATE_GAME: ActGame(); DrawGame(); break;
            case STATE_GAMEOVER: ActGameOver(); DrawGameOver(); break;
            case STATE_ENDING: DrawEnding(); break;
            case STATE_EXIT: QuitSDL(); exit(0);
        }

        ShowWindow();
        SDL_Delay(16);
    }
    return 0;
}

void InitGameOver(void) {}
void InitBullet(void) {}
void InitScoreBoard(void) {}
void InitSound(void) {}
void LoadSound(void) {}
void PlayBGM(void) {}
void InitTTF(void) {}