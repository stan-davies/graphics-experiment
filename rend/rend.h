#ifndef REND_H
#define REND_H

#include "util/util.h"

void init_rend(
        SDL_Window     *win
);

void set_rendcl(
        struct col      c
);

void rend_cl(
        void
);

void rend_rc(
        SDL_Rect        rc      ,
        struct col      c
);

void push_rend(
        void
);

#endif
