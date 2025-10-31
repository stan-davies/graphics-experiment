#ifndef SDL_UTIL_H
#define SDL_UTIL_H

#include "util/core.h"

int init_sdl(
        SDL_Window    **win
);

void end_sdl(
        SDL_Window    **win
);

#endif
