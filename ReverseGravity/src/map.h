#ifndef MAP_H
#define MAP_H

#include "defs.h"

#define ROOM_ROWS 2
#define ROOM_COLS 2

extern int current_room_row;
extern int current_room_col;

// 현재 로드된 방의 맵
extern int g_map_data[MAP_HEIGHT][MAP_WIDTH];

// 방 로드
void LoadRoom(int row, int col);

// 방 이동(drow, dcol)
void ChangeRoom(int drow, int dcol);

#endif