#include <math.h>

#include "../include/game.h"

float get_terrain_height(float x)
{
    return (float)(SCREEN_HEIGHT - 20 + 10 * sinf(x / 8));
}
