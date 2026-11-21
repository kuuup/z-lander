#ifndef GAME_H
#define GAME_H

// Our target platform has a small nice!nano LCD screen
#define SCREEN_WIDTH 68
#define SCREEN_HEIGHT 160

typedef struct {
    float x;
    float y;
} Pixel;

// represents state of current game 
typedef struct {
    // current ship postion
    Pixel position;
    
    // vertical and horizontal velocity
    float vx;
    float vy;

    // true when crashed
    int collision;

    // true when landed 
    int landed;

    // game is running 
    int running;

    // engines use fuel, save current fuel level
    float fuel;
} GameState;

void game_init();

// get game state
GameState *get_gs(void);

#endif
