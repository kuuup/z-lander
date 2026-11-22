// raylib as renderer for desktop use
#ifndef RENDER_H
#define RENDER_H

#include "game.h"

int key_left_pressed(void);
int key_right_pressed(void);
int key_main_pressed(void);
int key_reset_pressed(void);
int key_exit_pressed(void);

void init_renderer(void);
void begin_frame(void);
void end_frame(void);
void render_ship(void);
void render_main_thruster(void);
void render_left_thruster(void);
void render_right_thruster(void);
void render_terrain(void);
void render_telemetry(float vx, float vy, float fuel);
void render_text(const char *text);
int render(int stop);

#endif
