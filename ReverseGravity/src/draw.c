#include "defs.h"
#include <SDL2/SDL.h>

void RenderEntity(Entity *object); 
extern TTF_Font* font_normal;

// ----------------------------------------
// 맵 렌더링 함수 (수정됨)
// ----------------------------------------
void DrawMap(void) {
    SDL_Rect destRect = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_Texture* texture_to_draw = NULL;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tile_type = g_map_data[y][x];

            // ★ 회전 각도 변수 추가
            double angle = 0.0;
            SDL_RendererFlip flip = SDL_FLIP_NONE;

            // 1. 3번 타일이면 기존 가시 이미지를 쓰고, 180도 회전
            if (tile_type == TILE_SPIKE_REVERSE) {
                texture_to_draw = g_tile_textures[TILE_SPIKE]; 
                angle = 180.0;
            } else {
                texture_to_draw = g_tile_textures[tile_type];
                angle = 0.0;
            }
            
            if (tile_type != TILE_EMPTY && texture_to_draw != NULL) {
                destRect.x = x * TILE_SIZE - camera.x;
                destRect.y = y * TILE_SIZE - camera.y;

                if (destRect.x + TILE_SIZE < 0) continue;
                if (destRect.x > SCREEN_WIDTH) continue;
                if (destRect.y + TILE_SIZE < 0) continue;
                if (destRect.y > SCREEN_HEIGHT) continue;

                // ★ SDL_RenderCopyEx를 사용해서 회전(angle) 적용
                SDL_RenderCopyEx(app.renderer, texture_to_draw, NULL, &destRect, angle, NULL, flip);
            }
        }
    }
}

void DrawGame(void) {
    DrawMap();
    RenderEntity(&player); 
}

void DrawGameOver(void) 
{
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);

    const char* msg = "Press R to Restart";
    SDL_Color color = {255, 255, 255, 255}; 

    SDL_Surface* surface = TTF_RenderText_Solid(font_normal, msg, color);
    if (!surface) {
        printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);

    SDL_Rect dest;
    dest.w = surface->w;
    dest.h = surface->h;
    dest.x = (SCREEN_WIDTH - dest.w) / 2;
    dest.y = (SCREEN_HEIGHT - dest.h) / 2;

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderEntity(Entity *object)
{
    SDL_Rect r = object->pos;
    r.x -= camera.x;
    r.y -= camera.y;
    SDL_RenderCopy(app.renderer, object->texture, NULL, &r);
}

void ClearWindow(void) {
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);
}

void ShowWindow(void) {
    SDL_RenderPresent(app.renderer);
}