#include "../include/game.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{  
    // needed for terrain generation
    srand((unsigned int)time(NULL));
    game_init();
    while(game_tick())
    {
        // noop
    }
    return 0;
}
