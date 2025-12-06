#include "defs.h"
#include "init.h"
#include "player_collision.h"
#include "player_death.h"
#include "scene_title.h"

int get_tile_at(int tx, int ty)
{
    if (tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT)
        return TILE_EMPTY;

    return g_map_data[ty][tx];
}

int is_wall_tile(int t)
{
    return (t == TILE_FLOOR);  // 필요한 경우 여러 타일 추가 가능
}

int check_wall_collision(void)
{
    int x1 = player.pos.x;
    int x2 = player.pos.x + player.pos.w - 1;

    // 몸통 중앙 높이
    int mid_y = player.pos.y + player.pos.h / 2;
    int mid_ty = mid_y / TILE_SIZE;

    int tx1 = x1 / TILE_SIZE;
    int tx2 = x2 / TILE_SIZE;

    // 왼쪽 벽
    if (is_wall_tile(get_tile_at(tx1, mid_ty)))
        return 1;

    // 오른쪽 벽
    if (is_wall_tile(get_tile_at(tx2, mid_ty)))
        return 1;

    return 0;
}

void resolve_horizontal_collision(void)
{
    SDL_Rect p = player.pos;

    int left_x   = p.x;
    int right_x  = p.x + p.w - 1;
    int top_y    = p.y;
    int bottom_y = p.y + p.h - 1;

    // 플레이어 박스가 겹치는 타일 좌표
    int left_tx   = left_x   / TILE_SIZE;
    int right_tx  = right_x  / TILE_SIZE;
    int top_ty    = top_y    / TILE_SIZE;
    int bottom_ty = bottom_y / TILE_SIZE;

    // --- 왼쪽 벽 충돌 ---
    int t1 = get_tile_at(left_tx, top_ty);
    int t2 = get_tile_at(left_tx, bottom_ty);

    if (is_wall_tile(t1) || is_wall_tile(t2)) {
        // 왼쪽 타일 바로 오른쪽으로 플레이어 위치 조정
        player.pos.x = (left_tx + 1) * TILE_SIZE;
    }

    // --- 오른쪽 벽 충돌 ---
    t1 = get_tile_at(right_tx, top_ty);
    t2 = get_tile_at(right_tx, bottom_ty);

    if (is_wall_tile(t1) || is_wall_tile(t2)) {
        // 오른쪽 벽에 맞춰 플레이어를 바깥으로 밀어냄
        player.pos.x = right_tx * TILE_SIZE - player.pos.w;
    }
}

void resolve_vertical_tile_collision(void)
{
    SDL_Rect p = player.pos;

    int feet_x1 = p.x;
    int feet_x2 = p.x + p.w - 1;
    int feet_y  = p.y + p.h;          // 플레이어 발 바로 아래
    int head_y  = p.y - 1;            // 머리 바로 위

    int tx1 = feet_x1 / TILE_SIZE;
    int tx2 = feet_x2 / TILE_SIZE;
    int ty_feet = feet_y / TILE_SIZE;
    int ty_head = head_y / TILE_SIZE;

    // =========================
    // 1) 바닥 충돌 (중력 아래 방향)
    // =========================
    if (!player.gravity_inverted)
    {
        if (is_wall_tile(get_tile_at(tx1, ty_feet)) ||
            is_wall_tile(get_tile_at(tx2, ty_feet)))
        {
            // 바닥 타일 위에 서도록 위치 조정
            player.pos.y = ty_feet * TILE_SIZE - p.h;
            player.v_y = 0;
            player.is_grounded = 1;
            return;
        }
    }

    // =========================
    // 2) 천장 충돌 (중력 위 방향)
    // =========================
    else
    {
        if (is_wall_tile(get_tile_at(tx1, ty_head)) ||
            is_wall_tile(get_tile_at(tx2, ty_head)))
        {
            // 천장 타일에 머리가 닿았을 때 위치 조정
            player.pos.y = (ty_head + 1) * TILE_SIZE;
            player.v_y = 0;
            player.is_grounded = 1;
            return;
        }
    }

    // 여기에 오면 공중 상태
    player.is_grounded = 0;
}



void check_spike_collision(void)
{
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
            if (t == TILE_SPIKE) {
                kill_player();
                return;
            }
        }
    }
}

void check_goal_reach(void)
{
    int tx1 = player.pos.x / TILE_SIZE;
    int ty1 = player.pos.y / TILE_SIZE;
    int tx2 = (player.pos.x + player.pos.w) / TILE_SIZE;
    int ty2 = (player.pos.y + player.pos.h) / TILE_SIZE;

    // 플레이어 네 모서리가 goal에 닿았는지 검사
    if (g_map_data[ty1][tx1] == TILE_GOAL ||
        g_map_data[ty1][tx2] == TILE_GOAL ||
        g_map_data[ty2][tx1] == TILE_GOAL ||
        g_map_data[ty2][tx2] == TILE_GOAL)
    {
        InitEnding();            // ending.png 로드
        game_state = STATE_ENDING;
    }
}