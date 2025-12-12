#ifndef VECS_H
#define VECS_H

#include "util/defn/types.h"

float dist_to_pt(
        struct float2   u       ,
        struct float2   v
);

float dist_to_ln(
        struct float2   v1      ,
        struct float2   v2      ,
        struct float2   p
);

#endif
