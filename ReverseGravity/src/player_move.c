#include "defs.h"
#include "init.h"
#include "player_move.h"
#include "player_collision.h"
#include "map.h"

void move_player_horzontal(double dt)
{
    int dir = 0;

    if (app.key_left && !app.key_right)
        dir = -1;
    else if (app.key_right && !app.key_left)
        dir = 1;

    player.v_x = dir * MOVE_SPEED;

    // 원래 위치 저장
    int old_x = player.pos.x;

    // 먼저 이동
    player.pos.x += (int)(player.v_x * dt);

    // 이제 벽 충돌 체크
    if (check_wall_collision())
    {
        // 충돌했으면 이동 취소(되돌리기)
        player.pos.x = old_x;
    }
}



extern Entity player;  // 선언만 필요

void UpdatePlayer()
{
    int screen_w = MAP_WIDTH * TILE_SIZE;
    int screen_h = MAP_HEIGHT * TILE_SIZE;

    // -------------------------------------------
    // ★ 속도 적용 코드 제거함!
    // player.pos.x += player.v_x;
    // player.pos.y += player.v_y;
    // -------------------------------------------

    // 방 전환 처리만 남김

    // 오른쪽 화면 밖
    if (player.pos.x >= screen_w)
    {
        ChangeRoom(0, +1);
        player.pos.x = 0;
    }
    // 왼쪽 화면 밖
    else if (player.pos.x + player.pos.w <= 0)
    {
        ChangeRoom(0, -1);
        player.pos.x = screen_w - player.pos.w;
    }

    // 아래 화면 밖
    if (player.pos.y >= screen_h)
    {
        ChangeRoom(+1, 0);
        player.pos.y = 0;
    }
    // 위 화면 밖
    else if (player.pos.y + player.pos.h <= 0)
    {
        ChangeRoom(-1, 0);
        player.pos.y = screen_h - player.pos.h;
    }
}


