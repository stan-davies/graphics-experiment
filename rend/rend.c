#include "rend.h"

static struct {
        int             I       ;

        SDL_Renderer   *r       ;

        struct col      clr_col ;
} rend = {
        .I      =       FALSE   , 
        .r      =       NULL    ,
        .clr_col = { 255, 255, 255 }
};

void init_rend(
        SDL_Window     *win
) {
        rend.r = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);         
                                // handle flags in some other way?

        rend_cl();
        
        rend.I = TRUE;
}

void set_rendcl(
        struct col      c
) {
        // note use of Uint8, emphasis on 8, so we needn't ensure range
        rend.clr_col = c;
}

void rend_cl(
        void
) {
        SDL_SetRenderDrawColor(rend.r, 
                rend.clr_col.r, rend.clr_col.g, rend.clr_col.b, 255);
        SDL_RenderClear(rend.r);
}

void rend_rc(
        SDL_Rect        rc      ,
        struct col      c
) {
        if (!rend.I) {
                return;
        }

        SDL_SetRenderDrawColor(rend.r, c.r, c.g, c.b, 255);
        SDL_RenderFillRect(rend.r, &rc);
}

void push_rend(
        void
) {
        if (!rend.I) {
                return;
        }

        SDL_RenderPresent(rend.r);
}
