#ifndef RC_MAN
#define RC_MAN

#include "util/util.h"

#define RCERR_NONE      0
#define RCERR_DRAW_NF   1
#define RCERR_RECOL_NF  2
#define RCERR_REPOS_NF  3

void init_rc_man(
        void
);

void dest_rc_man(
        void
);

// Could probably be better =/
int create_rc(
        int             w       ,
        int             h       ,
        int             x       ,
        int             y       ,
        Uint8           r       ,
        Uint8           g       ,
        Uint8           b
);

void rc_recol(
        int             rid     ,
        Uint8           r       ,
        Uint8           g       ,
        Uint8           b
);

void rc_repos(
        int             rid     ,
        int             x       ,
        int             y
);

void draw_rc(
        int             rid
);

#endif
