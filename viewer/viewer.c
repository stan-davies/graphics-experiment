#include "viewer/viewer.h"

#include <math.h>

#define PI              3.141598f
#define FOV             PI / 2.f
#define NOT_VIS         -8.f

static struct {
        struct int2     pos     ;

        float           view    ;       // Angle of view offset from (0, 1), in
                                        // (-pi, pi].
} viewer;

static void adj_ang(
        float          *theta   ,
        float           delta
);

static float rel_ang(
        struct int2     v
);

static int v_vis(
        struct int2     vx      ,
        float          *r_ang
);

void rotate_view(
        float           offset
) {
        adj_ang(&viewer.view, offset);
}

static void adj_ang(
        float          *theta   ,
        float           delta
) {
        float n = *theta + delta;

        if (n <= -PI) {
                n += 2 * PI;
        }

        if (n > PI) {
                n -= 2 * PI;
        }

        *theta = n;
}

void init_viewer(
        struct int2     _pos    ,
        float           _view
) {
        viewer.pos = _pos;
        viewer.view = _view;
}

static float rel_ang(
        struct int2     v
) {
        struct int2 l = { v.x - viewer.pos.x, v.y - viewer.pos.y };
        if (0 == l.x) {
                return NOT_VIS;
        }

        float v_ang = atanf(l.y / l.x);
        float add   = -PI;

        if (l.x / l.y < 0) {
                v_ang *= -1;
                add = PI;
        }

        v_ang += add * (l.x < 0);

        adj_ang(&v_ang, viewer.view);        
        return v_ang;
}

static int v_vis(
        struct int2     vx      ,
        float          *r_ang
) {
        *r_ang = rel_ang(vx);
        return fabsf(*r_ang) <= FOV / 2.f;
}
                


int visible(
        struct fence    w
        // '-> int2 post1, post2
) {
        float ang_1;
        if (v_vis(w.post_1, &ang_1)) {
                return TRUE;
        }

        float ang_2;
        if (v_vis(w.post_2, &ang_2)) {
                return TRUE;
        }

        // Note if either angle were 0, it would be caught above.
        return ang_1 / ang_2 < 0;
}
