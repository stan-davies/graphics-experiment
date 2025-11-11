#ifndef VIEWER_H
#define VIEWER_H

#include "util/core.h"

void init_viewer(
        struct int2     _pos    ,
        float           _view
);

struct int2 rel_p(
        float           r       ,
        float           t
);

int vx_in_view(
        struct int2     v1      ,
        struct int2     v2
);

void calc_ext(
        struct int2     v1      ,
        struct int2     v2      ,
        struct float2  *extent  ,       // Left/right most extent.
        struct float2  *angs    ,       // Actual angles.
        float          *i_e             // Inbetween extent.
);

float calc_nrst(
        struct int2     v1      ,
        struct int2     v2
);

float calc_dist(
        struct int2     v
);

int spans_fov(
        struct float2   interval
);

void draw_viewer(
        void
);

// Returns whether the view has changed (i.e. by rotation or relocation).
int update_viewer(
        void
);

#endif
