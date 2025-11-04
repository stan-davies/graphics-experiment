#ifndef VIEWER_H
#define VIEWER_H

#include "util/core.h"

void init_viewer(
        struct int2     _pos    ,
        float           _view
);

float rel_ang(
        struct int2     v
);

int v_vis(
        struct int2     vx
);

#endif
