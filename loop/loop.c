#include "loop.h"

#include "util/core.h"

#include "rend/rend.h"
#include "viewer/viewer.h"
#include "world/world.h"
#include "key_man/key_man.h"

#define MAX_KEYS        8

void loop(
        void
) {
        SDL_Event e;

        for (;;) {
                rend_cl();

                while (SDL_PollEvent(&e)) {
                        if (SDL_QUIT == e.type) {
                                return;
                        }

                        if (SDL_KEYDOWN == e.type) {
                                if (SDLK_x == e.key.keysym.sym) {
                                        return;
                                } else {
                                        k_down(e.key.keysym.sym);
                                }
                        }

                        if (SDL_KEYUP == e.type) {
                                k_up(e.key.keysym.sym);
                        }
                }

                if (update_viewer()) {
                        update_world();
                }

                draw_viewer();
                draw_world();

                push_rend();
        }
}
