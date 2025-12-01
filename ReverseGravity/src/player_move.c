#include "defs.h"
#include "init.h"
#include "player_move.h"
#include "player_collision.h"

// void move_player_horzontal(double dt)
// {
//     int dir = 0;

//     if (app.key_left && !app.key_right)
//         dir = -1;
//     else if (app.key_right && !app.key_left)
//         dir = 1;

//     player.v_x = dir * MOVE_SPEED;
//     player.pos.x += (int)(player.v_x * dt);

//     // if (player.pos.x < 0)
//     //     player.pos.x = 0;
//     // if (player.pos.x + player.pos.w > SCREEN_WIDTH)
//     //     player.pos.x = SCREEN_WIDTH - player.pos.w;
//     resolve_horizontal_collision();
// }

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
