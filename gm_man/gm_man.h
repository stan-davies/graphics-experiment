#ifndef GM_MAN
#define GM_MAN

#include "util/util.h"


// GEOMETRY!!!

// We store geometry as a group of vertices and then render with
// SDL_RenderGeometry. So... we will want to have our linked list of 'shapes',
// each of which is composed of a set of vertices.

#define GMERR_NONE      0
#define GMERR_DRAW_NF   1
// What errors will we need?
#define GMERR_RECOL_NF  2
#define GMERR_REPOS_NF  3

void init_gm_man(
        void
);

void dest_gm_man(
        void
);

// What do we need here?
int create_gm(
        float          *vrt_dat ,
        SDL_Color       col
);

void gm_repos(
        int             gid     ,
        int             x       ,
        int             y
);
// --- 

void draw_gm(
        int             gid
);

#endif
