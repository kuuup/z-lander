#include "../include/game.h"
#include "../include/terrain.h"
#include <stdlib.h>

// height map 
static float terrain[SCREEN_WIDTH] = {0};

// generated with AI
// Connect two points with a nice parabola
// 
// Screen coordinates: y = 0 is top, larger y values are lower on screen.
// Positive k arches downward (larger y), negative k arches upward (smaller y).
static float eval_parabola(float x, float x1, float y1, float x2, float y2, float k)
{
    if (x2 == x1)
    {
        return y1;
    }

    float t = (x - x1) / (x2 - x1);
    float linear = y1 + t * (y2 - y1);
    float arch = 4.0f * t * (1.0f - t);

    return linear + k * arch;
}

int get_random_height(void)
{
    int min = SCREEN_HEIGHT / 1.5f;
    int max = SCREEN_HEIGHT - 10;
    return min + rand() % (max - min + 1);
}

int get_random_pad_position(void)
{
    int min = 10;
    int max = SCREEN_WIDTH - 10 - LANDING_PAD_WIDTH;
    return min + rand() % (max - min + 1);
}

// generate a random landing pad
// connect left and right side of the pad with a parabola
void generate_terrain(void)
{
    int left_height = get_random_height();
    int pad_height = get_random_height();
    int right_height = get_random_height();

    int pad_x = get_random_pad_position();

    // segment left from pad
    for (int i = 0; i < pad_x; i++)
    {
        terrain[i] = eval_parabola(i, 0, left_height, pad_x, pad_height, 10.0f);
    }

    // pad
    for (int i = pad_x; i < pad_x + LANDING_PAD_WIDTH; i++)
    {
        terrain[i] = pad_height;
    }

    // right side
    for (int i = pad_x + LANDING_PAD_WIDTH; i < SCREEN_WIDTH; i++)
    {
        terrain[i] = eval_parabola(i, pad_x + LANDING_PAD_WIDTH, pad_height, SCREEN_WIDTH - 1, right_height, 10.0f);
    }

}

// used by renderer for each terrain pixel
float get_terrain_height(float x)
{
    if (x < 0 || x >= SCREEN_WIDTH)
    {
        return 0;
    }
    return terrain[(int)x];
}
