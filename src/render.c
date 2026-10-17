#include "../include/game.h"

#include <raylib.h>
#include <stdio.h>

int render(GameState *gs)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    printf("%f", gs->x);
   
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - input keys");

    Vector2 ballPosition = { gs->x, gs->y };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 1.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 1.0f;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= 1.0f;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += 1.0f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawCircleV(ballPosition, 1, BLUE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
