#ifndef REND_H
#define REND_H

#include "imgio/imgio.h"

void init_rend(
        SDL_Window     *win
);

void set_rendcl(
        struct col      c
);

void rend_cl(
        void
);

SDL_Texture * rend_rq_tex(
        Uint32          format  ,
        int             access  ,
        // is it time for int2?
        int             w       ,
        int             h
);

void rend_tex(
        SDL_Rect       *dst     ,
        SDL_Texture    *tex
);

void rend_rc(
        SDL_Rect        rc      ,
        struct col      c
);

void push_rend(
        void
);

#endif
