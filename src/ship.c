#include "../include/game.h"
#include "../include/ship.h"

// relative pixel positions for our ship
// we start relative to center for easier collision testing
static const Pixel ship_pixels[] = {
    {  0,  0 },

    { -2,  1 }, {  0,  1 }, {  2,  1 },
    { -3,  2 }, { -1,  2 }, {  0,  2 }, {  1,  2 }, {  3,  2 },

    { -4,  3 }, { -1,  3 }, {  0,  3 }, {  1,  3 }, {  4,  3 },
    { -4,  4 }, { -3,  4 }, { -2,  4 }, {  2,  4 }, {  3,  4 }, {  4,  4 },
    { -3,  5 }, { -2,  5 }, {  2,  5 }, {  3,  5 },
    { -3,  6 }, { -2,  6 }, { -1,  6 }, {  1,  6 }, {  2,  6 }, {  3,  6 },

    { -2,  7 }, { -1,  7 }, {  0,  7 }, {  1,  7 }, {  2,  7 },
    { -2,  8 }, { -1,  8 }, {  0,  8 }, {  1,  8 }, {  2,  8 },
    { -1,  9 }, {  0,  9 }, {  1,  9 },

    { -1, 10 }, {  0, 10 }, {  1, 10 },
    {  0, 11 }
};

// use pixels when crashed
static const Pixel explosion_pixels[] = {
    {  0,  5 }, { -1,  5 }, {  1,  5 }, {  0,  6 }, {  0,  4 },
    
    { -2,  6 }, {  2,  6 }, { -2,  4 }, {  2,  4 },
    { -1,  7 }, {  1,  7 }, { -1,  3 }, {  1,  3 },
    
    { -3,  7 }, {  3,  7 }, { -3,  3 }, {  3,  3 },
    {  0,  8 }, {  0,  2 },
    { -2,  8 }, {  2,  8 }, { -2,  2 }, {  2,  2 },
    { -4,  5 }, {  4,  5 }, {  0,  9 }, {  0,  1 }
};

// use when engines are used
static const Pixel main_flame_pixels[] = {
    { -1, -1 }, {  0, -1 }, {  1, -1 },
    { -1, -2 }, {  0, -2 }, {  1, -2 },
                {  0, -3 }
};

static const Pixel left_flame_pixels[] = {
    { -5,  3 }, { -5,  4 },
    { -6,  3 }
};

static const Pixel right_flame_pixels[] = {
    {  5,  3 }, {  5,  4 },
    {  6,  3 }
};

int get_ship_pixel_length(void)
{
    GameState *gs = get_gs();
    if (gs->collision == 1)
    {
        return sizeof(explosion_pixels) / sizeof(explosion_pixels[0]);
    }
    return sizeof(ship_pixels) / sizeof(ship_pixels[0]);
}

// translate ship pixels to current ship position
Pixel get_ship_absolute_pixel(int index)
{
    GameState *gs = get_gs();
    float x = gs->position.x;
    float y = gs->position.y;
    if (gs->collision == 1)
    {
        return (Pixel){ explosion_pixels[index].x + x, y - explosion_pixels[index].y }; 
    }
    return (Pixel){ ship_pixels[index].x + x, y - ship_pixels[index].y }; 
}

int get_main_flame_length(void)
{
    return sizeof(main_flame_pixels) / sizeof(main_flame_pixels[0]);
}

// same as ship pixels but for engine
Pixel get_main_flame_pixel(int index)
{
    GameState *gs = get_gs();
    return (Pixel){ main_flame_pixels[index].x + gs->position.x, gs->position.y - main_flame_pixels[index].y };
}

int get_left_flame_length(void)
{
    return sizeof(left_flame_pixels) / sizeof(left_flame_pixels[0]);
}

// same as ship pixels but for engine
Pixel get_left_flame_pixel(int index)
{
    GameState *gs = get_gs();
    return (Pixel){ left_flame_pixels[index].x + gs->position.x, gs->position.y - left_flame_pixels[index].y };
}

int get_right_flame_length(void)
{
    return sizeof(right_flame_pixels) / sizeof(right_flame_pixels[0]);
}

// same as ship pixels but for engine
Pixel get_right_flame_pixel(int index)
{
    GameState *gs = get_gs();
    return (Pixel){ right_flame_pixels[index].x + gs->position.x, gs->position.y - right_flame_pixels[index].y };
}

