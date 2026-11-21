#include "game.h"

int key_left_pressed();
int key_right_pressed();
int key_main_pressed();
int key_reset_pressed();

void init_renderer();

void begin_frame(void);
void end_frame(void);
void render_ship(void);
void render_terrain(void);
void render_telemetry(float vx, float vy, float fuel);
int render(int stop);
