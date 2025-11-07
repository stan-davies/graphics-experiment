#include "util.h"

#include "sdl_util/sdl_util.h"
#include "rend/rend.h"
#include "gm_man/gm_man.h"
#include "viewer/viewer.h"
#include "world/world.h"

static SDL_Window *win;

int init(
        void
) {
        if (!init_log()) {
                printf("Error: Failed to initialise log file.\n");
                return FALSE;
        }

        win = NULL;

        if (!init_sdl(&win)) {
                log_err("Failed to initialise SDL.");
                return FALSE;
        }

        init_rend(win);

        // Where to define this colour?
        SDL_Color clr = { 0, 0, 0, 255 };
        set_rendcl(clr);

        init_gm_man();

        // Where to define these things?
        struct int2 vpos = { SCREEN_W / 2, SCREEN_H / 2 };
        init_viewer(vpos, 0.f);

        init_world();

        log_msg("Successfully initialsed program.");

        return TRUE;
}

void end(
        void
) {
        dest_world();
        dest_gm_man();
        end_sdl(&win);

        log_msg("Program ended without error.");
}
