#include "../include/game.h"
#include "../include/render.h"

#include <stdlib.h>

static GameState gs;

GameState *get_gs(void)
{
    return &gs;
}

void game_init(void)
{
    gs.position.x = 34.0f;
    gs.position.y = 10.0f;

    init_renderer();
    do
    {
        if (key_main_pressed())
        {
            gs.position.y += 1.0f;
        }

        if (key_left_pressed())
        {
            gs.position.x -= 1.0f;
        }

        if (key_right_pressed())
        {
            gs.position.x += 1.0f;
        }
    }
    while(render(get_gs()));
}

