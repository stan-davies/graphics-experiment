#include "util.h"

#include "sdl_util/sdl_util.h"
#include "rend/rend.h"
#include "viewer/viewer.h"
#include "world/world.h"
#include "key_man/key_man.h"

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

        // Where to define these things?
        struct float2 vpos = { SCREEN_W / 2.f, SCREEN_H / 2.f };
        init_viewer(vpos, 3.141598f);

        // Viewer must be initialised before this.
        init_world();

        init_key_man();

        log_msg("Successfully initialsed program.");

        return TRUE;
}

void end(
        void
) {
        dest_key_man();
        dest_world();
        end_sdl(&win);

        log_msg("Program ended without error.");
}
