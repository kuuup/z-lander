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


    SetTargetFPS(30);               // Set our game to run at 60 frames-per-second

}

int render(GameState *gs)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    // Main game loop
    if (WindowShouldClose())    // Detect window close button or ESC key
    {
        CloseWindow();        // Close window and OpenGL context
        return 0;
    }

    // Update
    //----------------------------------------------------------------------------------
    //if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 1.0f;
    //if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 1.0f;
    //if (IsKeyDown(KEY_UP)) ballPosition.y -= 1.0f;
    //if (IsKeyDown(KEY_DOWN)) ballPosition.y += 1.0f;
    //----------------------------------------------------------------------------------
    ballPosition.x = gs->position.x;
    ballPosition.y = gs->position.y;
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    for(int x = 0; x < SCREEN_WIDTH; x++)
    {
        DrawPixel(x, get_terrain_height((float)x), WHITE);
    }

    ClearBackground(BLACK);
    DrawPixelV(ballPosition, WHITE);

    EndDrawing();
        //----------------------------------------------------------------------------------

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

}
