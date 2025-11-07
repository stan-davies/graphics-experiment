#ifndef REND_H
#define REND_H

#include "imgio/imgio.h"
#include "util/core.h"

void init_rend(
        SDL_Window     *win
);

void set_rendcl(
        SDL_Color      c
);

void rend_cl(
        void
);

//SDL_Texture * rend_rq_tex(
//        Uint32          format  ,
//        int             access  ,
//        // is it time for int2?
//        int             w       ,
//        int             h
//);
//
//void rend_tex(
//        SDL_Rect       *dst     ,
//        SDL_Texture    *tex
//);
//
void rend_rc(
        SDL_Rect        rc      ,
        SDL_Color       c
);

void rend_ln(
        struct int2     origin  ,
        struct int2     head    ,
        SDL_Color       col
);

void rend_gm(
        SDL_Vertex     *vxs     ,
        int             vxs_n   ,
        int            *indices ,
        int             ind_n
);

void push_rend(
        void
);

#endif
