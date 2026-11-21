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
    gs.position.y = 32.0f;

    gs.vx = 0.0f;
    gs.vy = 0.0f;

    gs.collision = 0;
    gs.landed = 0;
    gs.running = 1;

    gs.fuel = 100.0f;

}

void game_init(void)
{
    reset_game();
    gs.running = 0;
    init_renderer();

    do
    {
        begin_frame();
        
        if (gs.running == 1 && gs.collision == 0 && gs.landed == 0)
        {
            if (gs.fuel > 0)
            {

                // set vertical velocity
                if (key_main_pressed())
                {
                    render_main_thruster();
                    gs.vy -= 0.03f;
                    gs.fuel -= 0.6f;
                }

                // set horizonal velocity
                if (key_left_pressed())
                {
                    render_right_thruster();
                    gs.vx -= 0.02f;
                    gs.fuel -= 0.4f;
                }

                if (key_right_pressed())
                {
                    render_left_thruster();
                    gs.vx += 0.02f;  
                    gs.fuel -= 0.4f;
                }
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
            if (gs.collision)
            {
                render_text("Game Over\n\npress\nENTER");
            }

            if (gs.landed)
            {
                render_text("Touchdown!\n\npress\nENTER");
            }

            if (!gs.running)
            {
                render_text("Start Game\n\npress\nENTER");
            }


            if (key_reset_pressed())
            {
                reset_game();
            }
        }
        render_terrain();
        render_ship();
        render_telemetry(gs.vx, gs.vy, gs.fuel);
        end_frame();
    }
    while(render(key_exit_pressed()));

}

