#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"
#include "rc_man/rc_man.h"
#include "tx_man/tx_man.h"
#include "subject/subject.h"

int main(
        void
) {
// init function
        SDL_Window  *win   = NULL;

        if (!init_sdl(&win)) {
                printf("Failed to initalise SDL.\n");
        }

        init_rend(win);
        init_rc_man();
        init_tx_man();

        struct col clr = { 255, 90, 120 };
        set_rendcl(clr);

        sub_init();

// main loop function
        SDL_Event e;
        int run = TRUE;
        while (run) {
                rend_cl();

                while (SDL_PollEvent(&e)) {
                        if (SDL_QUIT == e.type) {
                                run = FALSE;
                        } else if (SDL_KEYDOWN == e.type) {
                                if (SDLK_x == e.key.keysym.sym) {
                                        run = FALSE;
                                }
                        }

                        // update function will really only need `e.key`, which
                        // contains key pressed as well as key up/down,
                        // pressed/released, etc I have not decided how to deal
                        // with not looking at data if the event is not a
                        // keydown, so for know we send everyting and check in
                        // update function
                        // could attach a bool with 'keypress' but then that is
                        // almost just what we are doing anyway
                        // split only arises for stuff with passive behaviour
                        sub_update(e);
                }

                sub_draw();
                push_rend();
        }

// cleanup function
        dest_tx_man();
        dest_rc_man();
        end_sdl(&win);
        return 0;
}
