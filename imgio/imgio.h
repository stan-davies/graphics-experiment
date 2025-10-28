#ifndef COL_H
#define COL_H

#include "util/util.h"

struct col {
        Uint8           r       ;
        Uint8           g       ;
        Uint8           b       ;
};

struct img {
        int             w       ;
        int             h       ;
        struct col     *dat     ;
};

int read_img(
        char           *path    , 
        struct img     *read
);

#endif
