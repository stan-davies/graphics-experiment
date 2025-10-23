#include <stdio.h>

#include "util/util.h"

// expand this into a whole seperate thing with `rend` as object static to file
// along with background colour and whatever else
void rend_sq(SDL_Renderer *rend) {
        static int r = 30;
        static int g = 45;
        static int b = 240;
        static SDL_Rect rc = { 50, 50, 300, 300 };

        SDL_SetRenderDrawColor(rend, 255, 90, 120, 255);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, r, g, b, 255);
        SDL_RenderFillRect(rend, &rc);

        SDL_RenderPresent(rend);

        r = (g - 50) % 255; 
        g = (b - 20) % 255;
        b = (r + 20) % 255;
}


int main() {
        SDL_Window  *win   = NULL;

        if (!init_sdl(&win)) {
                printf("Failed to initalise SDL.\n");
        }

        SDL_Renderer* rend = NULL;
        rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

        rend_sq(rend);

        SDL_Event e;
        int run = TRUE;
        while (run) {
                while (SDL_PollEvent(&e)) {
                        if (SDL_QUIT == e.type) {
                                run = FALSE;
                        }
                        if (SDL_KEYDOWN == e.type) {
                                if (SDLK_c == e.key.keysym.sym) {
                                        rend_sq(rend);
                                } else { 
                                        run = FALSE;
                                }
                        }
                }
        }

        end_sdl(&win);
        return 0;
}
