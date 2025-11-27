// src/action.c (내용 전체 교체)
#include "defs.h"
#include "init.h"
#include<SDL2/SDL.h>

#define GRAVITY 2000.0
#define JUMP_SPEED 900.0
#define MOVE_SPEED 300.0

#define GROUND_Y (SCREEN_HEIGHT - TILE_SIZE)
#define CEILING_Y 0

//플레이어 리스폰 위치
#define RESPAWN_X (SCREEN_WIDTH / 4)
#define RESPAWN_Y (SCREEN_HEIGHT / 4)

//맵타일 읽기 헬퍼
static int get_title_at(int tx, int ty){
    if(tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT){
        return TILE_EMPTY;
    }

    return g_map_data[ty][tx];
}

// ----------------------------------------
// 액션(로직) 함수들
// ----------------------------------------

// 게임 진행 중 로직 (플레이어 이동, 충돌 등)
void ActGame(SDL_Event *event) {
    if(event->type == SDL_QUIT){

    }
    else if(event->type == SDL_KEYDOWN && !event->key.repeat){
        switch(event->key.keysym.sym){
            case SDLK_LEFT:
                app.key_left = 1;
                break;
            case SDLK_RIGHT:
                app.key_right = 1;
                break;

            case SDLK_SPACE: //space 키를 눌렀을 때 중력 반전
                player.gravity_inverted = !player.gravity_inverted;
                //속도 반전
                player.v_y = -player.v_y;
                break;

        }
    }
}

// 게임 오버 시 로직 (재시작 처리)
void ActGameOver(SDL_Event *event) {
    //죽은 상태에서만 호출됨
    if(app.key_r){
        //1.플레이어를 처음 상태로 되돌림
        InitPlayer();
        
        //2.추가로 리셋하고 싶은 상태들
        player.gravity_inverted = 0;
        player.v_x = 0.0;
        player.v_y = 0.0;
        player.is_grounded = 0;

        //3.키 상태 리셋
        app.key_down = 0;
        app.key_left = 0;
        app.key_r = 0;
        app.key_right = 0;
        app.key_up = 0;
    }
}

// ----------------------------------------
// 링커 오류 방지를 위한 더미/임시 함수 정의
// ----------------------------------------
// Dodge 게임에서 사용되었지만 현재는 필요 없는 함수들입니다.
void ActFinalScoreBoard(void) {}
void ActGameOverScreen(void) {}
void ActPlayer(void) {}
void ActBullet(void) {}
void ActScoreBoard(void) {}
void ActCheckDeath(void) {}


//플레이어 물리 업데이트(중력)

static void update_player(double dt){
    double g_dir = player.gravity_inverted ? -1.0 : 1.0;

    if(!player.is_grounded){
        player.v_y += GRAVITY * g_dir * dt;
    }
    player.pos.y += (int)(player.v_y *dt);
}

void resolve_vertical_collision(void){
    
    if(!player.gravity_inverted){
        //일반 중력 : 발이 바닥을 뚫지 않도록
        if(player.pos.y + player.pos.h >  GROUND_Y){//ground_y는 발이 닿을 수 있는 바닥 높이
            player.pos.y = GROUND_Y - player.pos.h;
            player.v_y = 0;
            player.is_grounded = 1;
        }
        else{
            //역중력: 머리가 천장을 뚫지 않도록
            if(player.pos.y < CEILING_Y){//ceiling_y 는 천장 높이
                player.pos.y = CEILING_Y;
                player.v_y = 0;
                player.is_grounded = 1;
            }
        }
    }
}

//뒤집혔을 때 캐릭터 뒤집어서 그리기
void draw_player(void){
    SDL_Rect dst = player.pos;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if(player.gravity_inverted){
        flip = SDL_FLIP_VERTICAL;
    }

    SDL_RenderCopyEx(app.renderer, player.texture, NULL, &dst, 0.0, NULL, flip);
}

//죽었을 때 처리 함수
static void kill_player(void){
    player.health = 0;
    player.v_x = 0.0;
    player.v_y = 0.0;

    //죽는 효과음
    if(death_effect){
        Mix_PlayChannel(-1, death_effect, 0);
    }

    // //바로 부활시키고 싶은 경우
    // player.pos.x = RESPAWN_X;
    // player.pos.y = RESPAWN_Y;
    // player.health = 1; //다시 살아남
}
void check_spike_collision(void){
    //플레이어의 바운딩 박스(px 단위)
    int x1 = player.pos.x;
    int y1 = player.pos.y;
    int x2 = player.pos.x + player.pos.w-1;
    int y2 = player.pos.y + player.pos.h-1;

    //모서리 네 점을 타일 좌표로 변환
    int tx1 = x1 /TILE_SIZE;
    int ty1 = y1 /TILE_SIZE;
    int tx2 = x2 /TILE_SIZE;
    int ty2 = y2 /TILE_SIZE;

    //네 모서리:(tx1,ty1), (tx1,ty2), (tx2,ty1), (tx2, ty2)
    int tiles_x[2] = {tx1, tx2};
    int tiles_y[2] = {ty1, ty2};
    
    for(int iy = 0; iy < 2; ++iy){
        for(int ix = 0;ix < 2 ; ++ix){
            int t = get_title_at(tiles_x[ix], tiles_y[iy]);
            if(t == TILE_SPIKE){
                kill_player();
                return;
            }
        }
    }
}