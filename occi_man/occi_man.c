#include "occi_man.h"

#include <stdlib.h>

#include "util/util.h"

#define MAX_INTS        16

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define RAD_TO_DEG(t) (t * 180.f / 3.141598f)

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

static void clean_occi(
        void
);

void init_occi_man(
        void
) {
        occi_man.occis = calloc(MAX_INTS, sizeof(struct float2));
        occi_man.intc  = 0;
}

void dest_occi_man(
        void
) {
        free(occi_man.occis);
        occi_man.occis = NULL;
}

///* closed */
//                // No idea if this works properly.
//                if (spans_fov(occ_int[0])) {
//                        break;
//                }

int get_seg(                            // Returns whether or not the wall was
                                        // split into multiple, discontinuous,
                                        // visible segments. This is also the
                                        // length of vints.

        struct float2   wall    ,       // Interval within FOV
        struct float2 **segs            // Visible segments to draw.
) {
        *segs = calloc(8, sizeof(struct float2));       // Freed by caller (I hope...)
        int segc = OCCI_WHID;

        (*segs)[segc++] = wall;

        for (int i = 0; i < occi_man.intc; ++i) {
                for (int s = 0; s < segc; ++s) {
                        if (ang_in_int((*segs)[s].x, occi_man.occis[i])) {
                                (*segs)[s].x = occi_man.occis[i].y;                        
                        }

                        if (ang_in_int((*segs)[s].y, occi_man.occis[i])) {
                                (*segs)[s].y = occi_man.occis[i].x;
                        }

//                        if (int_in_int(occi_man.occis[i], (*segs)[s])) {
//                                if (8 == segc) {
//                                        log_err("Too many splits");
//                                        continue;
//                                }
//
//// Rightmost segment runs from right of forewall to right of rearwall.
//                                (*segs)[segc].x = occi_man.occis[i].y;
//                                (*segs)[segc].y = (*segs)[s].y;
//
//// Leftmost segment runs from left of rearwall (as is) to left of forewall.
//                                (*segs)[s].y = occi_man.occis[i].x;
//
//                                segc++;
//                        }

                        if (!valid_int((*segs)[s])) {
                                log_msg("(%.0f, %.0f) not valid)", 
                                        RAD_TO_DEG((*segs)[s].x),
                                        RAD_TO_DEG((*segs)[s].y)
                                );
                                (*segs)[s] = (*segs)[segc--];
                        }
                }
        }

// Surely not the best way to do this since there is almost definitely overlap.
        for (int s = 0; s < segc; ++s) {
                occi_man.occis[occi_man.intc++] = (*segs)[s];
                if (occi_man.intc >= MAX_INTS) {
                        log_err("Maximum intervals reached... oh well!");
                }
        }

        clean_occi();

        for (int i = 0; i < occi_man.intc; ++i) {
                log_msg("(%.0f, %.0f)", 
                        RAD_TO_DEG(occi_man.occis[i].x), 
                        RAD_TO_DEG(occi_man.occis[i].y)
                );
        }
        log_msg("");
        
        return segc;
}

static void clean_occi(
        void
) {
        struct float2 c_int;
        for (int i = 0; i < occi_man.intc; ++i) {
                for (int j = 0; j < occi_man.intc; ++j) {
                        if (i == j) {
                                continue;
                        }

                        c_int = occi_man.occis[j];

// This could conceivably be total rubbish.
                        if (ang_in_int(occi_man.occis[i].x, c_int) 
                         || ang_in_int(occi_man.occis[i].y, c_int)) {
                                occi_man.occis[i].x = MAX(occi_man.occis[i].x, c_int.x);
                                occi_man.occis[i].y = MIN(occi_man.occis[i].y, c_int.y);

                                occi_man.occis[j] = occi_man.occis[--occi_man.intc];
                        }
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
        return (ang >= inter.x && ang <= inter.y);
}

static inline int int_in_int(
        struct float2   inner   ,
        struct float2   outer
) {
        return (outer.x <= inner.x && outer.y >= inner.y);
}
