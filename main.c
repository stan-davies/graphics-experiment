#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"
#include "gm_man/gm_man.h"

int main(
        void
) {
        if (!init()) {
                return 0;       // All required cleanup done internally.
        }

        struct col clr = { 255, 90, 120, 255 };
        set_rendcl(clr);

        float *vrts = calloc(4 * 2, sizeof(int));
        for (int i = 0; i < 4 * 2; ++i) {
                int x = i % 2 == 0 ? 
                                340 + 300 * (i % 4 / 2) : 
                                400 * (i > 4) + 80 * (1 == i || 7 == i);

                vrts[i] = (float)x;
        }

        SDL_Color c = { 30, 45, 240, 255 };

        int geo = create_gm(vrts, c);
        free(vrts);
        vrts = NULL;

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
                }

                draw_gm(geo);
                push_rend();
        }

        end();
        return 0;
}
