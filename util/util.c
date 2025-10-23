#include "util.h"

int init_sdl(SDL_Window **win) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                printf("Failed to initialise SDL.\n");
                return FALSE;
        }

        *win = SDL_CreateWindow("my window", 100, 100, 
                640, 480, SDL_WINDOW_SHOWN);
        if (!(*win)) {
                printf("Failed to create window.\n");
                return FALSE;
        }

        return TRUE;
}

void end_sdl(SDL_Window **win) {
       SDL_DestroyWindow(*win);
       *win   = NULL;
       SDL_Quit();
}
