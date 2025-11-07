#include "loop.h"

#include "util/core.h"

#include "rend/rend.h"
#include "viewer/viewer.h"
#include "world/world.h"

void loop(
        void
) {
        SDL_Event e;
        int run = TRUE;

        while (run) {
                rend_cl();

                while (SDL_PollEvent(&e)) {
                        if (SDL_QUIT == e.type) {
                                run = FALSE;
                        } else if (SDL_KEYDOWN == e.type) {
                                if (update_viewer(e.key)) {
                                        update_world();
                                }
                        
                                if (SDLK_x == e.key.keysym.sym) {
                                        run = FALSE;
                                }
                        }
                }

                draw_viewer();
                draw_world();

                push_rend();
        }
}
