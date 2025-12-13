#include "vecs.h"

#include <math.h>
#include "util/core.h"

static inline int in_interval(
        float           i_a     ,
        float           i_b     ,
        float           x
);

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
        if (0.f == v1.x - v2.x) {
                if (in_interval(v1.y, v2.y, p.y)) {
                        return fabsf(v1.x - p.x);
                }
                goto nr_endp;
        } else if (0.f == v1.y - v2.y) {
                if (in_interval(v1.x, v2.x, p.x)) {
                        return fabsf(v1.y - p.y);
                }
                goto nr_endp;
        }

        float lambda = ((v1.y - p.y) * (v2.y - v1.y)
                      - (v1.x - p.x) * (v2.x - v1.x))
                     / ((v2.x - v1.x) * (v2.x - v1.x)
                      + (v2.y - v1.y) * (v2.y - v1.y));
        
        if (lambda < 0.f || lambda > 1.f) {
                goto nr_endp;
        }

        struct float2 np = {
                .x = v1.x + lambda * (v2.x - v1.x),
                .y = v1.y + lambda * (v2.y - v1.y)
        };

        return dist_to_pt(np, p);

nr_endp:
        float d1 = dist_to_pt(v1, p);
        float d2 = dist_to_pt(v2, p);
        return MIN(d1, d2);
}

static inline int in_interval(
        float           i_a     ,
        float           i_b     ,
        float           x
) {
        // Greater than min and less than max.
        return (x > (i_a < i_b ? i_a : i_b)) && (x < (i_a < i_b ? i_b : i_a));
}
