#include "sdl_util.h"

// init all

// dest all

int init_sdl(
        SDL_Window     **win
) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                log_err("Failed to initialise SDL.");
                return FALSE;
        }

        *win = SDL_CreateWindow("Graphics Experiment", 100, 100, 
                                        640, 480, SDL_WINDOW_SHOWN);
        if (!(*win)) {
                log_err("Failed to create window.");
                return FALSE;
        }

        return TRUE;
}

void end_sdl(
        SDL_Window    **win
) {
        SDL_DestroyWindow(*win);
        *win = NULL;
        SDL_Quit();
}
