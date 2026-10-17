#ifndef GAME_H
#define GAME_H

// Our target platform has a small nice!nano LCD screen
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 68

typedef struct {
    float x;
    float y;
} GameState;

void game_init();
GameState *get_gs(void);

#endif
