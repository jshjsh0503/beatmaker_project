#include "defs.h"
#include "init.h"
#include "player_move.h"

#define MOVE_SPEED 300.0

void move_player_horzontal(double dt)
{
    int dir = 0;

    if (app.key_left && !app.key_right)
        dir = -1;
    else if (app.key_right && !app.key_left)
        dir = 1;

    player.v_x = dir * MOVE_SPEED;
    player.pos.x += (int)(player.v_x * dt);

    if (player.pos.x < 0)
        player.pos.x = 0;
    if (player.pos.x + player.pos.w > SCREEN_WIDTH)
        player.pos.x = SCREEN_WIDTH - player.pos.w;
}
