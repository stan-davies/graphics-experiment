#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"

// expand this into a whole seperate thing with `rend` as object static to file
// along with background colour and whatever else
void rend_sq() {
        static struct col       c       = { 30, 45, 240 };
        static SDL_Rect         rc      = { 50, 50, 300, 300 };

        rend_cl();

        rend_rc(rc, c);

        push_rend();

        c.r = (c.g - 50) % 255; 
        c.g = (c.b - 20) % 255;
        c.b = (c.r + 20) % 255;
}

int main() {
        SDL_Window  *win   = NULL;

        if (!init_sdl(&win)) {
                printf("Failed to initalise SDL.\n");
        }

        init_rend(win);

        struct col clr = { 255, 90, 120 };
        set_rendcl(clr);

        rend_sq();

        SDL_Event e;
        int run = TRUE;
        while (run) {
                while (SDL_PollEvent(&e)) {
                        if (SDL_QUIT == e.type) {
                                run = FALSE;
                        }
                        if (SDL_KEYDOWN == e.type) {
                                if (SDLK_c == e.key.keysym.sym) {
                                        rend_sq();
                                } else { 
                                        run = FALSE;
                                }
                        }
                }
        }

        end_sdl(&win);
        return 0;
}
