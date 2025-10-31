#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"
#include "tx_man/tx_man.h"
#include "subject/subject.h"
#include "imgio/imgio.h"

int main(
        void
) {
        if (!init()) {
                return 0;       // All required cleanup done internally.
        }

        struct col clr = { 255, 90, 120, 255 };
        set_rendcl(clr);

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

                if (TXERR_NONE != tx_man_poll_err()) {
                        log_err("Problem in texture manager.");
                        break;
                }

                sub_draw();
                push_rend();
        }

        end();
        return 0;
}
