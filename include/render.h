// raylib as renderer for desktop use
#ifndef RENDER_H
#define RENDER_H

#include "game.h"

int key_left_pressed();
int key_right_pressed();
int key_main_pressed();
int key_reset_pressed();
int key_exit_pressed();

void init_renderer();
void begin_frame(void);
void end_frame(void);
void render_ship(void);
void render_main_thruster(void);
void render_left_thruster(void);
void render_right_thruster();
void render_terrain(void);
void render_telemetry(float vx, float vy, float fuel);
void render_text(const char *text);
int render(int stop);

#endif
