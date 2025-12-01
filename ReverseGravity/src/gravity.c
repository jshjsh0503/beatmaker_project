#include "defs.h"
#include "init.h"
#include "gravity.h"
#include "player_collision.h"

#define GRAVITY 3000.0
#define GROUND_Y (SCREEN_HEIGHT - TILE_SIZE)
#define CEILING_Y 0

void gravity_inverted(double dt)
{
    double g_dir = player.gravity_inverted ? -1.0 : 1.0;

    if (!player.is_grounded)
        player.v_y += GRAVITY * g_dir * dt;

    player.pos.y += (int)(player.v_y * dt);
}

// void resolve_vertical_collision(void)
// {
//     if (!player.gravity_inverted)
//     {
//         if (player.pos.y + player.pos.h > GROUND_Y)
//         {
//             player.pos.y = GROUND_Y - player.pos.h;
//             player.v_y = 0;
//             player.is_grounded = 1;
//         }
//     }
//     else
//     {
//         if (player.pos.y < CEILING_Y)
//         {
//             player.pos.y = CEILING_Y;
//             player.v_y = 0;
//             player.is_grounded = 1;
//         }
//     }
// }

void resolve_vertical_collision(void)
{
    // player_collision.c 안에서 구현한 타일 기반 충돌을 호출
    resolve_vertical_tile_collision();
}