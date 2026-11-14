#include "../include/ship.h"

// Relative pixel positions for our ship
// We start at bottom center for easyier collision testing
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

Pixel* get_ship()
{
    return ship_pixels;
}

int get_ship_pixel_length()
{
    return sizeof(ship_pixels) / sizeof(ship_pixels[0]);
}

Pixel get_ship_absolute_pixel(int index, float x, float y)
{
    return (Pixel){ ship_pixels[index].x + x, y - ship_pixels[index].y }; 
}

