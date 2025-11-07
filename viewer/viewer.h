#ifndef VIEWER_H
#define VIEWER_H

#include "util/core.h"

void init_viewer(
        struct int2     _pos    ,
        float           _view
);

int vx_in_view(
        struct int2     v1      ,
        struct int2     v2
);

void calc_ext(
        struct int2     v1      ,
        struct int2     v2      ,
        float          *lm_e    ,       // Leftmost extent.
        float          *rm_e    ,       // Rightmost extent.
        float          *i_e             // Inbetween extent.
);

void draw_viewer(
        void
);

// Returns whether the view has changed (i.e. by rotation or relocation).
int update_viewer(
        void
);

#endif
