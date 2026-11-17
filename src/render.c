#include "../include/game.h"
#include "../include/terrain.h"
#include "../include/ship.h"

#include <raylib.h>
#include <stdio.h>

static Vector2 ballPosition = { 0, 0 };

int key_left_pressed(void) {
    if (IsKeyDown(KEY_LEFT))
    {
        return 1;
    }
    return 0;
}

int key_right_pressed(void) {
    if (IsKeyDown(KEY_RIGHT))
    {
        return 1;
    }
    return 0;
}

int key_main_pressed(void) {
    if (IsKeyDown(KEY_UP))
    {
        return 1;
    }
    return 0;
}

int key_reset_pressed(void) {
    if (IsKeyDown(KEY_ENTER))
    {
        return 1;
    }
    return 0;
}

void init_renderer(void)
{
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "z-lander");
    SetTargetFPS(30);
}

int render()
{
    if (WindowShouldClose()) {
        CloseWindow();
        return 0;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    for(int x = 0; x < SCREEN_WIDTH; x++)
    {
        DrawPixel(x, get_terrain_height((float)x), WHITE);
    }

    int length = get_ship_pixel_length();
    for(int i = 0; i < length; i++)
    {
        Pixel position = get_ship_absolute_pixel(i); //gs->position.x, gs->position.y); 
        ballPosition.x = position.x;
        ballPosition.y = position.y;

        DrawPixelV(ballPosition, WHITE);
    }

    EndDrawing();
}
