#ifndef VIEWER_H
#define VIEWER_H

#include "util/core.h"

void init_viewer(
        struct int2     _pos    ,
        float           _view
);

int visible(
        struct int2     v1      ,
        struct int2     v2
);

void draw_viewer(
        void
);

void update_viewer(
        SDL_KeyboardEvent       k
);

#endif
