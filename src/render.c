#include "../include/game.h"
#include "../include/terrain.h"

#include <raylib.h>
#include <stdio.h>

static Vector2 ballPosition = { 0, 0 };

int key_left_pressed() {
    if (IsKeyDown(KEY_LEFT))
    {
        return 1;
    }
    return 0;
}

int key_right_pressed() {
    if (IsKeyDown(KEY_RIGHT))
    {
        return 1;
    }
    return 0;
}

int key_main_pressed() {
    if (IsKeyDown(KEY_UP))
    {
        return 1;
    }
    return 0;
}

void init_renderer()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "z-lander");
    SetTargetFPS(30);
}

int render(GameState *gs)
{
    if (WindowShouldClose()) {
        CloseWindow();
        return 0;
    }

    ballPosition.x = gs->position.x;
    ballPosition.y = gs->position.y;
    
    BeginDrawing();
    for(int x = 0; x < SCREEN_WIDTH; x++)
    {
        DrawPixel(x, get_terrain_height((float)x), WHITE);
    }

    ClearBackground(BLACK);
    DrawPixelV(ballPosition, WHITE);

    EndDrawing();
}
