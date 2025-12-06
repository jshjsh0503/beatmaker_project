#include "defs.h"
#include "init.h"
#include "player_collision.h"
#include "player_death.h"
#include "scene_title.h"

int get_tile_at(int tx, int ty) {
    if (tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT) return TILE_EMPTY;
    return g_map_data[ty][tx];
}

int is_wall_tile(int t) {
    return (t == TILE_FLOOR);
}

int check_wall_collision(void) {
    int x1 = player.pos.x;
    int x2 = player.pos.x + player.pos.w - 1;
    int mid_y = player.pos.y + player.pos.h / 2;
    int mid_ty = mid_y / TILE_SIZE;
    int tx1 = x1 / TILE_SIZE;
    int tx2 = x2 / TILE_SIZE;

    if (is_wall_tile(get_tile_at(tx1, mid_ty))) return 1;
    if (is_wall_tile(get_tile_at(tx2, mid_ty))) return 1;
    return 0;
}

void resolve_horizontal_collision(void) {
    SDL_Rect p = player.pos;
    int left_x = p.x;
    int right_x = p.x + p.w - 1;
    int top_y = p.y;
    int bottom_y = p.y + p.h - 1;

    int left_tx = left_x / TILE_SIZE;
    int right_tx = right_x / TILE_SIZE;
    int top_ty = top_y / TILE_SIZE;
    int bottom_ty = bottom_y / TILE_SIZE;

    int t1 = get_tile_at(left_tx, top_ty);
    int t2 = get_tile_at(left_tx, bottom_ty);

    if (is_wall_tile(t1) || is_wall_tile(t2)) {
        player.pos.x = (left_tx + 1) * TILE_SIZE;
    }

    t1 = get_tile_at(right_tx, top_ty);
    t2 = get_tile_at(right_tx, bottom_ty);

    if (is_wall_tile(t1) || is_wall_tile(t2)) {
        player.pos.x = right_tx * TILE_SIZE - player.pos.w;
    }
}

void resolve_vertical_tile_collision(void) {
    SDL_Rect p = player.pos;
    int feet_x1 = p.x;
    int feet_x2 = p.x + p.w - 1;
    int feet_y = p.y + p.h;
    int head_y = p.y - 1;

    int tx1 = feet_x1 / TILE_SIZE;
    int tx2 = feet_x2 / TILE_SIZE;
    int ty_feet = feet_y / TILE_SIZE;
    int ty_head = head_y / TILE_SIZE;

    if (!player.gravity_inverted) {
        if (is_wall_tile(get_tile_at(tx1, ty_feet)) || is_wall_tile(get_tile_at(tx2, ty_feet))) {
            player.pos.y = ty_feet * TILE_SIZE - p.h;
            player.v_y = 0;
            player.is_grounded = 1;
            return;
        }
    } else {
        if (is_wall_tile(get_tile_at(tx1, ty_head)) || is_wall_tile(get_tile_at(tx2, ty_head))) {
            player.pos.y = (ty_head + 1) * TILE_SIZE;
            player.v_y = 0;
            player.is_grounded = 1;
            return;
        }
    }
    player.is_grounded = 0;
}

// ★ 가시 충돌 체크 (2번, 3번 모두 확인)
void check_spike_collision(void) {
    int x1 = player.pos.x;
    int y1 = player.pos.y;
    int x2 = player.pos.x + player.pos.w - 1;
    int y2 = player.pos.y + player.pos.h - 1;

    int tx1 = x1 / TILE_SIZE;
    int ty1 = y1 / TILE_SIZE;
    int tx2 = x2 / TILE_SIZE;
    int ty2 = y2 / TILE_SIZE;

    int tiles_x[2] = { tx1, tx2 };
    int tiles_y[2] = { ty1, ty2 };

    for (int iy = 0; iy < 2; ++iy) {
        for (int ix = 0; ix < 2; ++ix) {
            int t = get_tile_at(tiles_x[ix], tiles_y[iy]);
            // 2번(바닥 가시) 또는 3번(천장 가시)이면 사망
            if (t == TILE_SPIKE || t == TILE_SPIKE_REVERSE) {
                kill_player();
                return;
            }
        }
    }
}

void check_goal_reach(void) {
    int tx1 = player.pos.x / TILE_SIZE;
    int ty1 = player.pos.y / TILE_SIZE;
    int tx2 = (player.pos.x + player.pos.w) / TILE_SIZE;
    int ty2 = (player.pos.y + player.pos.h) / TILE_SIZE;

    if (g_map_data[ty1][tx1] == TILE_GOAL || g_map_data[ty1][tx2] == TILE_GOAL ||
        g_map_data[ty2][tx1] == TILE_GOAL || g_map_data[ty2][tx2] == TILE_GOAL) {
        InitEnding();
        game_state = STATE_ENDING;
    }
}