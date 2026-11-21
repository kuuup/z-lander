#include "../include/game.h"
#include "../include/render.h"
#include "../include/terrain.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static GameState gs;

GameState *get_gs(void)
{
    return &gs;
}

int check_collision(void)
{
    float x = gs.position.x;
    float height = get_terrain_height(x);

    // check for velocity
    // we can not land when ship is too fast
    if (fabsf(gs.vx) > 0.3f || fabsf(gs.vy) > 0.3f)
    {
        return 1;
    }

    // check if we landed on a flat surface
    for (int i = x - LANDING_PAD_WIDTH / 2; i < x + LANDING_PAD_WIDTH / 2; i++)
    {
        if (fabsf(get_terrain_height(i) - height) > 1)
        {
            return 1;
        }
    }

    return 0;
}

// check if game is over
void set_game_state(void)
{
    float x = gs.position.x;
    float height = get_terrain_height(x);

    if (x <= 0 || x >= SCREEN_WIDTH - 1)
    {
        gs.collision = 1;
        return;
    }

    if (gs.position.y >= height) {
        gs.collision = check_collision();
        if (!gs.collision)
        {
            // Yay :) Touchdown
            gs.landed = 1;
        }
    }
}

void reset_game(void)
{
    // ship starts here 
    gs.position.x = SCREEN_WIDTH / 2;
    gs.position.y = 32.0f;

    // ship is floating
    gs.vx = 0.0f;
    gs.vy = 0.0f;

    // no crash, not landed
    gs.collision = 0;
    gs.landed = 0;

    // active round after reset
    gs.running = 1;

    // full tank :)
    gs.fuel = 100.0f;

    // create new terrain for each new game
    generate_terrain();
}

void game_init(void)
{
    reset_game();
    gs.running = 0;
    init_renderer();

    do
    {

        // control render loop 
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
                    gs.fuel -= 0.9f;
                }

                // set horizontal velocity
                if (key_left_pressed())
                {
                    render_right_thruster();
                    gs.vx -= 0.02f;
                    gs.fuel -= 0.6f;
                }

                if (key_right_pressed())
                {
                    render_left_thruster();
                    gs.vx += 0.02f;  
                    gs.fuel -= 0.6f;
                }
            }
            
            // gravity
            gs.vy += 0.01f;
            
            // set position 
            gs.position.y += gs.vy;
            gs.position.x += gs.vx;

            set_game_state();
            render_terrain();
            render_ship();
            render_telemetry(gs.vx, gs.vy, gs.fuel);
        }
        else
        {
            if (gs.collision)
            {
                render_text("Crash!\nGame Over\n\npress\nENTER");
                render_terrain();
                render_ship();

            }

            if (gs.landed)
            {
                render_terrain();
                render_ship();
                render_text("Touchdown!\n\npress\nENTER");
            }

            if (!gs.running)
            {
                render_text("Start Game\n\npress\nENTER");
            }

            // restart game
            if (key_reset_pressed())
            {
                reset_game();
            }
        }

        // control render loop 
        end_frame();
    }
    // run while window is open and exit key is not pressed
    while(render(key_exit_pressed()));

}

