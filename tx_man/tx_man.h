#ifndef RC_MAN
#define RC_MAN

#include "util/util.h"

#define TXERR_DRAW_NF   1
#define TXERR_REPOS_NF  2

void init_tx_man(
        void
);

void dest_tx_man(
        void
);

// Could probably be better =/
int create_tx(
        int             w       ,
        int             h       ,
        int             x       ,
        int             y       ,
        // ^ associated dstrect?
        // something something pixel data
);

// there was a recolour function, not really needed
// if we had some kind of animated texture, would that be a single texture that
// we update, or several textures that we cycle through?

void tx_repos(
        int             tid     ,
        int             x       ,
        int             y
        // ^ change internal dstrect
);

void draw_tx(
        int             tid
);

#endif
