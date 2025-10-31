#include "util.h"

#include "sdl_util/sdl_util.h"
#include "rend/rend.h"
#include "tx_man/tx_man.h"
#include "subject/subject.h"

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

        init_tx_man();

        if (!sub_init()) {
                dest_tx_man();
                end_sdl(&win);
        
                log_err("Failed to initialise subject.");
                return FALSE;
        }

        log_msg("Successfully initialsed program.");

        return TRUE;
}

void end(
        void
) {
        dest_tx_man();
        end_sdl(&win);

        log_msg("Program ended.");
}
