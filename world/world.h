#ifndef WORLD_H
#define WORLD_H

#include "util/core.h"

void init_world(
        void
);

void dest_world(
        void
);

void recalc_world(
        void
);

void update_world(
        void
);

void check_mclick(
        SDL_MouseButtonEvent    m
);

void check_mmove(
        SDL_MouseMotionEvent    m
);

void draw_world(
        void
);

#endif
