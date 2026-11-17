#include "../include/game.h"
#include "../include/render.h"

#include <stdlib.h>
#include <stdio.h>

static GameState gs;

GameState *get_gs(void)
{
    return &gs;
}

void game_init(void)
{
    gs.position.x = 34.0f;
    gs.position.y = 10.0f;

    gs.vx = 0.0f;
    gs.vy = 0.0f;

    init_renderer();

    do
    {
        // gravitiy
        gs.vy += 0.01f;

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
        
        // set position 
        gs.position.y += gs.vy;
        gs.position.x += gs.vx;
        
        if (gs.position.y >= SCREEN_HEIGHT)
        {
            gs.position.y = 10.0f;
            gs.vy = 0.0;
        }

    }
    while(render(get_gs()));
}

