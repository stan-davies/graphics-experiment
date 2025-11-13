#include "viewer/viewer.h"

#include <math.h>

#include "rend/rend.h"
#include "key_man/key_man.h"

#define PI              3.141598f
#define FOV             PI / 2.f
#define NOT_VIS         -8.f

#define MOVE_BY         5
#define ROTATE_BY       PI / 135.f

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)

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
        t = viewer.view - t;

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

        viewer.lp_la.x = viewer.pos.x + 500 * cosf(viewer.view + FOV / 2.f);
        viewer.lp_la.y = viewer.pos.y - 500 * sinf(viewer.view + FOV / 2.f);

        viewer.rp_la.x = viewer.pos.x + 500 * cosf(viewer.view - FOV / 2.f);
        viewer.rp_la.y = viewer.pos.y - 500 * sinf(viewer.view - FOV / 2.f);
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
        if (ang1 <= FOV / 2) {
                return TRUE;
        }

        float ang2 = fabsf(rel_ang(v2));
        if (ang2 <= FOV / 2) {
                return TRUE;
        }

        return FALSE;
}

void calc_ext(
        struct int2     v1      ,
        struct int2     v2      ,
        struct float2  *extent  ,       // Left/right most extent.
        struct float2  *angs    ,       // Actual angles.
        float          *i_e             // Inbetween extent.
) {
        // Corresponds to vertices.
        angs->x = rel_ang(v1);
        angs->y = rel_ang(v2);

        // Strict left and right.
        float angl = MIN(angs->x, angs->y);
        float angr = MAX(angs->x, angs->y);

        // Something wrong with calculation for when both are -ive.
        extent->x = MIN( fabsf(angl), FOV / 2.f ) * angl / fabsf(angl);
        extent->y = MIN( fabsf(angr), FOV / 2.f ) * angr / fabsf(angr);

        *i_e = MAX(0.f, extent->y - extent->x);
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
        return interval.x == -FOV / 2.f && interval.y == FOV / 2.f;
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

        set_draw();
        return TRUE;
}
