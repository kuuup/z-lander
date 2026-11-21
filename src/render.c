#include "../include/game.h"
#include "../include/terrain.h"
#include "../include/ship.h"

#include <raylib.h>
#include <stdio.h>

static Vector2 pointerPosition = { 0, 0 };

// check for pressed keys
// used by game loop
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

int key_exit_pressed(void) {
    if (IsKeyDown(KEY_ESCAPE))
    {
        return 1;
    }
    return 0;
}

void init_renderer(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "z-lander");
    SetTargetFPS(30);
}

void begin_frame(void)
{
    BeginDrawing();
    ClearBackground(BLACK);
}

void end_frame(void)
{
    EndDrawing();
}

// render methods iterate over ship and terrain pixels
void render_ship(void)
{
    int length = get_ship_pixel_length();
    for(int i = 0; i < length; i++)
    {
        Pixel position = get_ship_absolute_pixel(i); 
        pointerPosition.x = position.x;
        pointerPosition.y = position.y;

        DrawPixelV(pointerPosition, WHITE);
    }
}

void render_main_thruster(void)
{
    int length = get_main_flame_length();
    for(int i = 0; i < length; i++)
    {
        Pixel position = get_main_flame_pixel(i);
        pointerPosition.x = position.x;
        pointerPosition.y = position.y;

        DrawPixelV(pointerPosition, WHITE);
    }
}

void render_left_thruster(void)
{
    int length = get_left_flame_length();
    for(int i = 0; i < length; i++)
    {
        Pixel position = get_left_flame_pixel(i);
        pointerPosition.x = position.x;
        pointerPosition.y = position.y;

        DrawPixelV(pointerPosition, WHITE);
    }
}

void render_right_thruster(void)
{
    int length = get_right_flame_length();
    for(int i = 0; i < length; i++)
    {
        Pixel position = get_right_flame_pixel(i);
        pointerPosition.x = position.x;
        pointerPosition.y = position.y;

        DrawPixelV(pointerPosition, WHITE);
    }
}

void render_terrain(void)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        DrawPixel(x, get_terrain_height((float)x), WHITE);
    }
}

// render telemetry on top of screen (velocity and fuel level)
void render_telemetry(float vx, float vy, float fuel)
{

    // fuel as bar (normalized to SCREEN_WIDTH width)
    if (fuel <= 0)
    {
        DrawText("No fuel!", 1, 1, 5, WHITE);
    }
    else
    {
        DrawRectangle(0, 0, (SCREEN_WIDTH * fuel) / 100, 5, WHITE);
    }
        
    // velocity as text
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X:%.2f Y:%.2f", vx, vy);
    DrawText(buffer, 2, 10, 5, WHITE);
}

// draw text in main menu
void render_text(const char *text)
{
    DrawText(text, 6, 6, 5, WHITE);
}

// executed by game loop
int render(int stop)
{
    if (stop || WindowShouldClose())
    {
        CloseWindow();
        return 0;
    }
    return 1;
}
