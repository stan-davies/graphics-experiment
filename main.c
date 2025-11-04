#include <stdio.h>

#include "util/util.h"
#include "rend/rend.h"
#include "gm_man/gm_man.h"
#include "viewer/viewer.h"

int main(
        void
) {
        if (!init()) {
                return 0;       // All required cleanup done internally.
        }

//        struct col clr = { 255, 90, 120, 255 };
        struct col clr = { 0, 0, 0, 255 };
        set_rendcl(clr);

//        float *vrts = calloc(4 * 2, sizeof(int));
//        for (int i = 0; i < 4 * 2; ++i) {
//                int x = i % 2 == 0 ? 
//                                340 + 300 * (i % 4 / 2) : 
//                                400 * (i > 4) + 80 * (1 == i || 7 == i);
//
//                vrts[i] = (float)x;
//        }
//
//        SDL_Color c = { 30, 45, 240, 255 };
//
//        int geo = create_gm(vrts, c);
//        free(vrts);
//        vrts = NULL;

        struct fence f1 = {
                { 50, 200 },
                { 400, 50 }
        };

        SDL_Color vis = { 0, 255, 0, 255 };
        SDL_Color hid = { 255, 0, 0, 255 };
        SDL_Color use;

        struct int2 o = { 320, 240 };
        struct int2 l = { 320 + 50 * cosf(1.57f), 240 + 50 * sinf(1.57f) };
        SDL_Color c = { 30, 45, 240, 255 };

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

                rend_ln(o, l, c);

                f1.post_2.x--;
                if (50 == f1.post_2.x) {
                        f1.post_2.x = 400;
                }

                if (visible(f1)) {
                        use = vis;
                } else {
                        use = hid;
                }

                rend_ln(f1.post_1, f1.post_2, use);
                
//                draw_gm(geo);
                push_rend();
        }

        end();
        return 0;
}
