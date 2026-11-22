#ifndef SHIP_H
#define SHIP_H

#include "game.h"

int get_ship_pixel_length(void);
Pixel get_ship_absolute_pixel(int index);

int get_main_flame_length(void);
Pixel get_main_flame_pixel(int index);

int get_left_flame_length(void);
Pixel get_left_flame_pixel(int index);

int get_right_flame_length(void);
Pixel get_right_flame_pixel(int index);

#endif
