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
void render_terrain(void);
void render_telemetry(float vx, float vy, float fuel);
void render_text(char *text1, char *text2, char *text3);
int render(int stop);
