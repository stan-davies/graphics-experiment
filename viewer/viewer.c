#include "viewer/viewer.h"

#include <math.h>

#include "rend/rend.h"
#include "key_man/key_man.h"

#define PI              3.141598f
#define FOV             PI / 2.f
#define HFOV            FOV / 2.f
#define NOT_VIS         -8.f

#define MOVE_BY         5
#define ROTATE_BY       PI / 200.f

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)
#define DIF(a, b) (MAX(a, b) - MIN(a, b))
#define CNT(a, b) (MIN(fabsf(a), fabsf(b)))

static struct {
        struct int2     pos     ;

        float           view    ;       // Angle of view offset from (0, 1), in
                                        // (-pi, pi].

// ---  for drawing
        struct int2     lookat  ;
        SDL_Rect        body    ;
        
        struct int2     lp_la   ;
        struct int2     rp_la   ;
} viewer;

static SDL_Color bod_col = { 30, 45, 240, 255 };
static SDL_Color prong_col = { 255, 90, 120, 255 };

static void adj_ang(
        float          *theta   ,
        float           delta
);

static float rel_ang(
        struct int2     v
);

static void set_draw(
        void
);

void init_viewer(
        struct int2     _pos    ,
        float           _view
) {
        viewer.pos = _pos;
        viewer.view = _view;

        set_draw();

        viewer.body.w = 10;
        viewer.body.h = 10;
}

struct int2 rel_p(
        float           r       ,
        float           t
) {
        adj_ang(&t, viewer.view);

        struct int2 p = {
                .x = viewer.pos.x + r * cosf(t),
                .y = viewer.pos.y - r * sinf(t)
        };

        return p;
}

static void set_draw(
        void
) {
        viewer.lookat.x = viewer.pos.x + 500 * cosf(viewer.view);
        viewer.lookat.y = viewer.pos.y - 500 * sinf(viewer.view);

        viewer.body.x = viewer.pos.x - 5;
        viewer.body.y = viewer.pos.y - 5;

        viewer.lp_la.x = viewer.pos.x + 500 * cosf(viewer.view + HFOV);
        viewer.lp_la.y = viewer.pos.y - 500 * sinf(viewer.view + HFOV);

        viewer.rp_la.x = viewer.pos.x + 500 * cosf(viewer.view - HFOV);
        viewer.rp_la.y = viewer.pos.y - 500 * sinf(viewer.view - HFOV);
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

static float rel_ang(
        struct int2     v
) {
        struct int2 l = { v.x - viewer.pos.x, viewer.pos.y - v.y };
        if (0 == l.x) {
                return NOT_VIS;
        }

        float quo   = (float)l.y / (float)l.x;
        float v_ang = atanf(fabsf(quo));
        float add   = -PI;

        if (quo < 0.f) {
                v_ang *= -1.f;
                add = PI;
        }

        v_ang += add * (l.x < 0);

        adj_ang(&v_ang, -viewer.view);

        return v_ang;
}

int vx_in_view(
        struct int2     v1      ,
        struct int2     v2
) {
        float ang1 = fabsf(rel_ang(v1));
        if (ang1 <= HFOV) {
                return TRUE;
        }

        float ang2 = fabsf(rel_ang(v2));
        if (ang2 <= HFOV) {
                return TRUE;
        }

        return FALSE;
}

void calc_ext(
        struct int2     v1      ,
        struct int2     v2      ,
        struct float2  *extent  ,       // Left/right most extent.
        struct float2  *angs    ,       // Angles to vertices.
        float          *i_e             // Inbetween extent.
) {
        angs->x = rel_ang(v1);
        angs->y = rel_ang(v2);

        extent->x = CNT(angs->x, HFOV) * angs->x / fabsf(angs->x);
        extent->y = CNT(angs->y, HFOV) * angs->y / fabsf(angs->y);

        // Fixes issue for when vertex passes behind the viewer.
        if (DIF(angs->x, angs->y) >= PI) {
                if (MAX(fabsf(extent->x), fabsf(extent->y)) == extent->x) {
                        extent->x = -HFOV * extent->x / fabsf(extent->x);
                } else {
                        extent->y = -HFOV * extent->y / fabsf(extent->y);
                }
        }

        *i_e = MAX(0.f, DIF(extent->x, extent->y) - 0.05f);
}

float calc_nrst(
        struct int2     v1      ,
        struct int2     v2
) {
        float lambda = (float)((v1.x - v2.x) * (v1.x - viewer.pos.x)
                      + (v1.y - v2.y) * (v1.y - viewer.pos.y))
                     / (float)((v2.x - v1.x) * (v2.x - v1.x)
                      + (v2.y - v1.y) * (v2.y - v1.y));

        if (lambda < 0.f) {
                lambda = 0.f;
        } else if (lambda > 1.f) {
                lambda = 1.f;
        }

        struct int2 to_near_p = {
                .x = (v1.x + lambda * (v2.x - v1.x)) - viewer.pos.x,
                .y = (v1.y + lambda * (v2.y - v1.y)) - viewer.pos.y
        };

        return sqrtf(to_near_p.x * to_near_p.x + to_near_p.y * to_near_p.y);
}

float calc_dist(
        struct int2     v
) {
        struct int2 dif = {
                .x = v.x - viewer.pos.x,
                .y = v.y - viewer.pos.y
        };

        return sqrt(dif.x * dif.x + dif.y + dif.y);
}

int spans_fov(
        struct float2   interval
) {
        return interval.x == -HFOV && interval.y == HFOV;
}

void draw_viewer(
        void
) {
        rend_rc(viewer.body, bod_col);
        rend_ln(viewer.pos, viewer.lookat, bod_col);

        rend_ln(viewer.pos, viewer.lp_la, prong_col);
        rend_ln(viewer.pos, viewer.rp_la, prong_col);
}

int update_viewer(
        void
) {
        SDL_Keycode k;
        int i = 0;

        while (-1 != (k = get_key(i++))) {
                switch (k) {
                case SDLK_w:
                        if (viewer.pos.y > MOVE_BY) {
                                viewer.pos.y -= MOVE_BY;
                        }
                        break;
                case SDLK_s:
                        if (viewer.pos.y < SCREEN_H - MOVE_BY) {
                                viewer.pos.y += MOVE_BY;
                        }
                        break;
                case SDLK_a:
                        if (viewer.pos.x > MOVE_BY) {
                                viewer.pos.x -= MOVE_BY;
                        }
                        break;
                case SDLK_d:
                        if (viewer.pos.x < SCREEN_W - MOVE_BY) {
                                viewer.pos.x += MOVE_BY;
                        }
                        break;
                case SDLK_e:
                        adj_ang(&viewer.view, -ROTATE_BY);
                        break;
                case SDLK_q:
                        adj_ang(&viewer.view, ROTATE_BY);
                        break;
                default:
                        return FALSE;
                }
        }

        if (1 == i) {           // Indicates no keys were pressed.
                return FALSE;
        }

        set_draw();
        return TRUE;
}

// Works really well for horizontal walls that are near and terribly for
// everything else.
// In terms of vertical walls, some kind of problem in y components?
// In terms of far, why are they going off of the end? - A problem with
// whenever the angle is not simply the FOV bounds.
void points_on_line(
        struct int2     p1      ,
        struct int2     p2      ,
        struct float2   exts    ,
        struct int2    *a1      ,
        struct int2    *a2
) {
        struct float2 v = {
                (float)viewer.pos.x,
                (float)viewer.pos.y
        };
        float num, den, lambda;

        adj_ang(&exts.x, -viewer.view);
        adj_ang(&exts.y, -viewer.view);


        // For a1:
        num = p1.y - v.y + (v.x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
        den = sinf(exts.x) - cosf(exts.x) * (p2.y - p1.y) / (p2.x - p1.x);

        lambda = num / den;

        a1->x = v.x + lambda * cosf(exts.x);
        a1->y = v.y + lambda * sinf(exts.x);


        // for a2:
        num = p1.y - v.y + (v.x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
        den = sinf(exts.y) - cosf(exts.y) * (p2.y - p1.y) / (p2.x - p1.x);

        lambda = num / den;

        a2->x = v.x + lambda * cosf(exts.y);
        a2->y = v.y + lambda * sinf(exts.y);
}
