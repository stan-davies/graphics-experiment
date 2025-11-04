#include "util.h"

#include "sdl_util/sdl_util.h"
#include "rend/rend.h"
#include "gm_man/gm_man.h"

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

        init_gm_man();

        log_msg("Successfully initialsed program.");

        return TRUE;
}

void end(
        void
) {
        dest_gm_man();
        end_sdl(&win);

        log_msg("Program ended.");
}
