#ifndef VIEWER_H
#define VIEWER_H

#include "util/core.h"

struct fence {
        struct int2     post_1  ;
        struct int2     post_2  ;
};

void init_viewer(
        struct int2     _pos    ,
        float           _view
);

int visible(
        struct fence    w
        // '-> int2 post1, post2
);

#endif
