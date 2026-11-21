#ifndef GAME_H
#define GAME_H

// Our target platform has a small nice!nano LCD screen
#define SCREEN_WIDTH 68
#define SCREEN_HEIGHT 160

typedef struct {
    float x;
    float y;
} Pixel;

typedef struct {
    Pixel position;
    float vx;
    float vy;
    int collision;
    int landed;
    int running;
    float fuel;
} GameState;

void game_init();
GameState *get_gs(void);

#endif
