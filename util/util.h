#ifndef UTIL_H
#define UTIL_H

#include "sdl-inc.h"
#include "col.h"

#define TRUE    1
#define FALSE   0
#ifndef NULL
#define NULL    ((void *)0)
#endif

int init_sdl(
        SDL_Window **win
);

void end_sdl(
        SDL_Window **win
);

#endif
