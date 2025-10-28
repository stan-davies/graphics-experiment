#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"
#include "rc_man/rc_man.h"
#include "subject/subject.h"

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

        sub_init();

        rend_cl();
        sub_draw();
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
                                        sub_update();

                                        rend_cl();
                                        sub_draw();
                                        push_rend();

                                } else { 
                                        run = FALSE;
                                }
                        }
                }
        }

        dest_rc_man();
        end_sdl(&win);
        return 0;
}
