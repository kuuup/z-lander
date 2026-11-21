#include "../include/game.h"
#include "../include/render.h"
#include "../include/terrain.h"
#include <stdlib.h>
#include <stdio.h>

static GameState gs;

GameState *get_gs(void)
{
    return &gs;
}

void set_game_state(void)
{
    float x = gs.position.x;

    if (x <= 0 || x >= SCREEN_WIDTH)
    {
        gs.collision = 1;
    }
    
    float height = get_terrain_height(x);
    if (gs.position.y >= height) {
        if (gs.vx <= 0.3f && gs.vy <= 0.3f)
        {
            // Yay :)
            gs.landed = 1;
        }
        else
        {
            // Boooo :(
            gs.collision = 1;
        }
    }
}

void reset_game(void)
{
    gs.position.x = 34.0f;
    gs.position.y = 10.0f;

    gs.vx = 0.0f;
    gs.vy = 0.0f;

    gs.collision = 0;
    gs.landed = 0;

    gs.fuel = 100.0f;
}

void game_init(void)
{
    reset_game();
    init_renderer();

    do
    {

        if (gs.collision == 0 && gs.landed == 0)
        {

            printf("fuel: %f\n", gs.fuel);

            if (gs.fuel > 0)
            {

                // set vertical velocity
                if (key_main_pressed())
                {
                    gs.vy -= 0.03f;
                    gs.fuel -= 0.3f;
                }

                // set horizonal velocity
                if (key_left_pressed())
                {
                    gs.vx -= 0.02f;
                    gs.fuel -= 0.2f;
                }

                if (key_right_pressed())
                {
                    gs.vx += 0.02f;  
                    gs.fuel -= 0.2f;
                }
            }
            else
            {
                printf("no fuel");
            }

            // gravitiy
            gs.vy += 0.01f;
            
            // set position 
            gs.position.y += gs.vy;
            gs.position.x += gs.vx;

            set_game_state();
        }
        else
        {
            if (key_reset_pressed())
            {
                reset_game();
            }
        }
        begin_frame();
        render_terrain();
        render_ship();
        render_telemetry(gs.vx, gs.vy, gs.fuel);
        end_frame();
    }
    while(render(key_exit_pressed()));

}

