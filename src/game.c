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

int is_collision(void)
{
    float x = gs.position.x;

    if (x <= 0 || x >= SCREEN_WIDTH)
    {
        return 1;
    }
    
    float height = get_terrain_height(x);
    if (gs.position.y >= height)
    {
        return 1;
    }
    return 0;
}

void reset_game(void)
{
    gs.position.x = 34.0f;
    gs.position.y = 10.0f;

    gs.vx = 0.0f;
    gs.vy = 0.0f;

    gs.collision = 0;
    gs.landed = 0;
}

void game_init(void)
{
    reset_game();
    init_renderer();

    do
    {

        if (gs.collision == 0 && gs.landed == 0)
        {
 
            // set vertical velocity
            if (key_main_pressed())
            {
                gs.vy -= 0.03f;
            }

            // set horizonal velocity
            if (key_left_pressed())
            {
                gs.vx -= 0.02f;  
            }

            if (key_right_pressed())
            {
                gs.vx += 0.02f;  
            }

            // gravitiy
            gs.vy += 0.01f;
            
            // set position 
            gs.position.y += gs.vy;
            gs.position.x += gs.vx;

            gs.collision = is_collision();
            if (is_collision())
            {
                printf("collision\n");
            }
            
            if (gs.position.y >= SCREEN_HEIGHT)
            {
                gs.position.y = 10.0f;
                gs.vy = 0.0;
            }
        }
        else
        {
            if (key_reset_pressed())
            {
                reset_game();
            }
        }

    }
    while(render());

}

