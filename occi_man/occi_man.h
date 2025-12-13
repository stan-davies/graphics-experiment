#ifndef OCCI_MAN_H
#define OCCI_MAN_H

#include "util/defn/types.h"

void init_occi_man(
        void
);

void ready_occi_man(
        void
);

void dest_occi_man(
        void
);

int occi_closed(
        void
);

int get_seg(                            // Returns number of segments.
        struct float2   wall    ,       // Interval within FOV, left/right
                                        // aligned.
        struct float2 **vints           // Visible intervals
);

#endif
