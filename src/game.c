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
    gs.x = 80.0f;
    gs.y = 10.0f;
    render(get_gs());
}

