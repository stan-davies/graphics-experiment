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

        struct col clr = { 255, 90, 120, 255 };
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

        struct int2 o = { 320, 240 };
        float lambda = 70.f;
        float theta = 0.f;

        struct int2 l = { 0, 0 };
        float pi = 3.141598;

        SDL_Color vis = { 0, 255, 0, 255 };
        SDL_Color hid = { 255, 0, 0, 255 };
        SDL_Color use;

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

                l.x = o.x + lambda * cosf(theta);
                l.y = o.y + lambda * sinf(theta);

                theta += 2.f * pi / 720.f;
                if (theta == 2.f * pi) {
                        theta = 0.f;
                }

                if (v_vis(l)) {
                        use = vis;
                } else {
                        use = hid;
                }

                rend_ln(o, l, use);
                
//                draw_gm(geo);
                push_rend();
        }

        end();
        return 0;
}
