#include "occi_man.h"

#include <stdlib.h>

#include "util/util.h"
#include "viewer/viewer.h"

#define MAX_INTS        16

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

static struct {
        struct float2  *occis   ;       // Occupied intervals.
        int             intc    ;       // Number of ...
        
} occi_man;

static inline int valid_int(
        struct float2   inter
);

static inline int ang_in_int(
        float           ang     ,
        struct float2   inter           // Should be left/right aligned.
);

static inline int int_in_int(
        struct float2   inner   ,
        struct float2   outer
);

static void add_occis(
        struct float2  *segs    ,
        int             segc
);

void init_occi_man(
        void
) {
        occi_man.occis = calloc(MAX_INTS, sizeof(struct float2));
        occi_man.intc  = 0;
}

void ready_occi_man(
        void
) {
        occi_man.intc = 0;
}

void dest_occi_man(
        void
) {
        free(occi_man.occis);
        occi_man.occis = NULL;
}

int occi_close(
        void
) {
        // No idea if this works properly.
        return spans_fov(occi_man.occis[0]);
}

int get_seg(                            // Returns whether or not the wall was
                                        // split into multiple, discontinuous,
                                        // visible segments. This is also the
                                        // length of vints.

        struct float2   wall    ,       // Interval within FOV
        struct float2 **segs            // Visible segments to draw.
) {
                                // Freed by caller (I hope...)
        *segs = calloc(8, sizeof(struct float2));
        int segc = 0;

        (*segs)[segc++] = wall;

        for (int i = 0; i < occi_man.intc; ++i) {
                for (int s = 0; s < segc; ++s) {
                        if (ang_in_int((*segs)[s].x, occi_man.occis[i])) {
                                (*segs)[s].x = occi_man.occis[i].y;                        
                        }

                        if (ang_in_int((*segs)[s].y, occi_man.occis[i])) {
                                (*segs)[s].y = occi_man.occis[i].x;
                        }

                        if (int_in_int(occi_man.occis[i], (*segs)[s])) {
                                if (8 == segc) {
                                        log_err("Too many splits");
                                        continue;
                                }

// Rightmost segment runs from right of forewall to right of rearwall.
                                (*segs)[segc].x = occi_man.occis[i].y;
                                (*segs)[segc].y = (*segs)[s].y;

// Leftmost segment runs from left of rearwall (as is) to left of forewall.
                                (*segs)[s].y = occi_man.occis[i].x;

                                segc++;
                        }

                        if (!valid_int((*segs)[s])) {
                                (*segs)[s] = (*segs)[segc--];
                        }
                }
        }

        add_occis(*segs, segc);

        return segc;
}

static void add_occis(
        struct float2  *segs    ,
        int             segc
) {
        struct float2 c_int;
        int added = FALSE;
        for (int s = 0; s < segc; ++s) {
                for (int i = 0; i < occi_man.intc; ++i) {
                        c_int = occi_man.occis[i];

                        if (ang_in_int(segs[s].x, c_int) 
                         || ang_in_int(segs[s].y, c_int)) {
                                occi_man.occis[i].x = MAX(segs[s].x, c_int.x);
                                occi_man.occis[i].y = MIN(segs[s].y, c_int.y);
                                added = TRUE;
                        }
                }

                if (!added) {
                        if (MAX_INTS == occi_man.intc) {
                                log_err("Too many intervals!");
                                continue;
                        }
                        occi_man.occis[occi_man.intc++] = segs[s];
                }
        }
}

static inline int valid_int(
        struct float2   inter
) {
        return (inter.x > inter.y);
}

static inline int ang_in_int(
        float           ang     ,
        struct float2   inter
) {
        return (ang <= inter.x && ang >= inter.y);
}

static inline int int_in_int(
        struct float2   inner   ,
        struct float2   outer
) {
        return (outer.x >= inner.x && outer.y <= inner.y);
}
