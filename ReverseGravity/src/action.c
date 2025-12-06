#include "player_move.h"
#include "gravity.h"
#include "player_collision.h"
#include "player_death.h"
#include "player_draw.h"
#include "defs.h"
#include "action.h"
#include "map.h"

#define JUMP_SPEED 900.0

void ActGame(void)
{
    const double dt = 1.0 / 60.0;

    if (app.key_space && player.is_grounded)
    {
        player.gravity_inverted = !player.gravity_inverted;

        if (player.gravity_inverted)
            player.texture = player_texture_reverse;
        else
            player.texture = player_texture_normal;

        app.key_space = 0;
    }

    move_player_horzontal(dt);

    if (app.key_down && player.is_grounded)
    {
        double jump_dir = player.gravity_inverted ? 1.0 : -1.0;
        player.v_y = JUMP_SPEED * jump_dir;
        player.is_grounded = 0;
        app.key_down = 0;
    }

    gravity_inverted(dt);
    resolve_vertical_collision();
    check_spike_collision();
    check_goal_reach();

    // ★ 방 바깥 나갔는지 체크
    UpdatePlayer();

    // ★ 카메라 끄기(고정)
    camera.x = 0;
    camera.y = 0;
}



void ActGameOver(void) {
    if(app.key_r){
        InitPlayer();
        player.gravity_inverted = 0;
        player.v_x = 0.0;
        player.v_y = 0.0;
        player.is_grounded = 0;

        // app.key_down = 0;
        // app.key_left = 0;
        // app.key_r = 0;
        // app.key_right = 0;
        // app.key_up = 0;
        // app.key_r = 0;

        ResetInputFlags();   // ← ★ 모든 입력을 한 번에 초기화 (가장 안전함)
        game_state = STATE_GAME;
    }
}
