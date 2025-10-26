#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"
#include "rc_man/rc_man.h"

int main(
        void
) {
        SDL_Window  *win   = NULL;

        if (!init_sdl(&win)) {
                printf("Failed to initalise SDL.\n");
        }

        init_rend(win);
        init_rc_man();

        struct col clr = { 255, 90, 120 };
        set_rendcl(clr);

        struct col c = { 30, 45, 240 };
        int rc_id = create_rc(400, 200, 50, 50, c.r, c.g, c.b);

        rend_cl();
        draw_rc(rc_id);
        push_rend();

        SDL_Event e;
        int run = TRUE;
        while (run) {
                while (SDL_PollEvent(&e)) {
                        if (SDL_QUIT == e.type) {
                                run = FALSE;
                        }
                        if (SDL_KEYDOWN == e.type) {
                                if (SDLK_c == e.key.keysym.sym) {
                                        c.r = (c.g - 50) % 255; 
                                        c.g = (c.b - 20) % 255;
                                        c.b = (c.r + 20) % 255;
                                        rc_recol(rc_id, c.r, c.g, c.b);

                                        rend_cl();
                                        draw_rc(rc_id);
                                        push_rend();

                                } else { 
                                        run = FALSE;
                                }
                        }
                }
        }

        end_sdl(&win);
        return 0;
}
