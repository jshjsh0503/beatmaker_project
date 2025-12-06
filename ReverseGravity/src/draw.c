// src/draw.c (파일 맨 위를 다음과 같이 수정해야 합니다.)
#include "defs.h"
#include <SDL2/SDL.h>

// DrawGame에서 사용되는 함수의 프로토타입을 미리 선언합니다.
// 이 선언을 추가하면 DrawGame에서 RenderEntity를 사용할 때 경고가 사라집니다.
void RenderEntity(Entity *object); 

extern TTF_Font* font_normal;

// ... (DrawMap 함수 등 기존 코드 유지)
// ----------------------------------------
// 맵 렌더링 함수 (새로 추가)
// ----------------------------------------
void DrawMap(void) {
    SDL_Rect destRect = {0, 0, TILE_SIZE, TILE_SIZE};
    SDL_Texture* texture_to_draw = NULL;

    // 맵 배열의 세로 (y)와 가로 (x)를 순회
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int tile_type = g_map_data[y][x];

            // 1. 그릴 텍스처 선택
            texture_to_draw = g_tile_textures[tile_type];
            
            // 빈 공간(0)이 아니며 텍스처가 로드되었을 경우에만 그림
            if (tile_type != TILE_EMPTY && texture_to_draw != NULL) {
                // 2. 그려질 화면상의 위치 계산
                // destRect.x = x * TILE_SIZE;
                // destRect.y = y * TILE_SIZE;
                
                // // 3. 텍스처를 렌더러에 복사 (화면에 그리기)
                // SDL_RenderCopy(app.renderer, texture_to_draw, NULL, &destRect);
                destRect.x = x * TILE_SIZE - camera.x;
                destRect.y = y * TILE_SIZE - camera.y;

                if (destRect.x + TILE_SIZE < 0) continue;
                if (destRect.x > SCREEN_WIDTH) continue;
                if (destRect.y + TILE_SIZE < 0) continue;
                if (destRect.y > SCREEN_HEIGHT) continue;

                SDL_RenderCopy(app.renderer, texture_to_draw, NULL, &destRect);
            }
        }
    }
}

// ----------------------------------------
// DrawGame 함수 수정 (맵 렌더링 호출)
// ----------------------------------------
void DrawGame(void) {
    DrawMap();
    RenderEntity(&player); // 플레이어 그리기 함수 호출 (기존 코드 활용)
}

// ----------------------------------------
// DrawGameOver 함수는 임시로 비워둠 (링커 오류 방지)
// ----------------------------------------
void DrawGameOver(void) 
{
    // 1) 배경을 검은색으로 채움
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);

    // 2) 출력할 텍스트 설정
    const char* msg = "Press R to Restart";

    SDL_Color color = {255, 255, 255, 255};  // 흰색

    // 3) SDL_ttf로 문자열 렌더링하여 surface 생성
    SDL_Surface* surface = TTF_RenderText_Solid(font_normal, msg, color);
    if (!surface) {
        printf("TTF_RenderText_Solid error: %s\n", TTF_GetError());
        return;
    }

    // 4) surface → texture 변환
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);

    // 5) 출력 위치 설정
    SDL_Rect dest;
    dest.w = surface->w;
    dest.h = surface->h;
    dest.x = (SCREEN_WIDTH - dest.w) / 2;   // 화면 중앙
    dest.y = (SCREEN_HEIGHT - dest.h) / 2;  // 화면 중앙

    // 6) 화면에 렌더링
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);

    // 7) 리소스 정리
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// // RenderEntity 함수 (SDL_RenderCopy를 호출하는 핵심 함수)
// void RenderEntity(Entity *object) {
//     SDL_RenderCopy(app.renderer, object->texture, NULL, &(object->pos));
//     return;
// }
void RenderEntity(Entity *object)
{
    SDL_Rect r = object->pos;
    r.x -= camera.x;
    r.y -= camera.y;

    SDL_RenderCopy(app.renderer, object->texture, NULL, &r);
}

// 렌더러를 초기 색상으로 지웁니다 (렌더링 시작)
void ClearWindow(void) {
    // 검은색으로 화면을 지우도록 설정
    SDL_SetRenderDrawColor(app.renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(app.renderer);
}

// 렌더링 결과를 화면에 표시합니다 (렌더링 끝)
void ShowWindow(void) {
    SDL_RenderPresent(app.renderer);
}
