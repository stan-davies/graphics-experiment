#ifndef KEY_MAN_H
#define KEY_MAN_H

#include "util/core.h"

void init_key_man(
        void
);

void dest_key_man(
        void
);

void k_down(
        SDL_Keycode     k
);

void k_up(
        SDL_Keycode     k
);

SDL_Keycode get_key(
        int             i
);

#endif
