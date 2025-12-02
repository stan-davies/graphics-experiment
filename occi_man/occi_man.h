#ifndef OCCI_MAN_H
#define OCCI_MAN_H

#include "util/defn/types.h"

#define OCCI_WSPLIT     2       // Wall was split.
#define OCCI_WWHOLE     1       // Wall remains whole.
#define OCCI_WHID       0       // Completely hidden.

// Called per render.
void init_occi_man(
        void
);

void dest_occi_man(
        void
);
// -- 

int get_vis(                            // Returns whether or not the wall was
                                        // split into multiple, discontinuous,
                                        // visible segments. This is also the
                                        // length of vints.

        struct float2   wall    ,       // Interval within FOV, left/right
                                        // aligned.
        struct float2 **vints           // Visible intervals
);

#endif
