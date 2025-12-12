#include "vecs.h"

#include <math.h>
#include "util/core.h"

float dist_to_pt(
        struct float2   u       ,
        struct float2   v
) {
        struct float2 dif = {
                .x = v.x - u.x,
                .y = v.y - u.y
        };

        return sqrtf(dif.x * dif.x + dif.y * dif.y);
}

float dist_to_ln(
        struct float2   v1      ,
        struct float2   v2      ,
        struct float2   p
) {
        float lambda = ((v1.y - p.y) * (v2.y - v1.y)
                      - (v1.x - p.x) * (v2.x - v1.x))
                     / ((v2.x - v1.x) * (v2.x - v1.x)
                      + (v2.y - v1.y) * (v2.y - v1.y));

        if (lambda < 0.f || lambda > 1.f) {
                float d1 = dist_to_pt(v1, p);
                float d2 = dist_to_pt(v2, p);
                return d1 < d2 ? d1 : d2;
        }

        struct float2 np = {
                .x = v1.x + lambda * (v2.x - v1.x),
                .y = v1.y + lambda * (v2.y - v1.y)
        };

        return dist_to_pt(np, p);
}
