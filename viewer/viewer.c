#include "viewer/viewer.h"

#include <math.h>

#include "rend/rend.h"
#include "key_man/key_man.h"
#include "util/vecs/vecs.h"
#include "world/world.h"

#define PI              3.141598f

#define FOV             PI / 2.f
#define HFOV            FOV / 2.f
#define FOCAL_L         50.f

#define LOS             SCREEN_H / 2.f          // Line of sight, i.e.
                                                // screenspace y-coord to put
                                                // centre of walls at.
#define NOT_VIS         -8.f

#define MOVE_BY         5
#define ROTATE_BY       PI / 200.f

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)
#define DIF(a, b) (MAX(a, b) - MIN(a, b))
#define CNT(a, b) (MIN(fabsf(a), fabsf(b)))

static struct {
        struct float2   pos     ;

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
        struct float2   _pos    ,
        float           _view
) {
        viewer.pos = _pos;
        viewer.view = _view;

        set_draw();

        viewer.body.w = 10;
        viewer.body.h = 10;
}

struct float2 rel_p(
        float           r       ,
        float           t
) {
        adj_ang(&t, viewer.view);

        struct float2 p = {
                .x = viewer.pos.x + r * cosf(t),
                .y = viewer.pos.y - r * sinf(t)
        };

        return p;
}

float get_los(
        void
) {
        return LOS;
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
        struct float2 l = { (float)v.x - viewer.pos.x, viewer.pos.y - (float)v.y };
        if (0 == l.x) {
                return NOT_VIS;
        }

        float quo   = l.y / l.x;
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

float rel_dist_pt(
        struct int2     p
) {
        struct float2 pf = { (float)p.x, (float)p.y };
        return dist_to_pt(pf, viewer.pos);

}

float rel_dist_ln(
        struct int2     v1      ,
        struct int2     v2
) {
        struct float2 v1f = { (float)v1.x, (float)v1.y };
        struct float2 v2f = { (float)v2.x, (float)v2.y };
        return dist_to_ln(v1f, v2f, viewer.pos);
}


int spans_fov(
        struct float2   interval
) {
        return interval.x == -HFOV && interval.y == HFOV;
}

void draw_viewer(
        void
) {
        struct int2 vp = { viewer.pos.x, viewer.pos.y };
        rend_rc(viewer.body, bod_col);
        rend_ln(vp, viewer.lookat, bod_col);

        rend_ln(vp, viewer.lp_la, prong_col);
        rend_ln(vp, viewer.rp_la, prong_col);
}

int update_viewer(
        void
) {
        SDL_Keycode k;
        int i = 0;

        struct float2 newp = viewer.pos;

        while (-1 != (k = get_key(i++))) {
                switch (k) {
                case SDLK_w:
                        newp = rel_p(MOVE_BY, 0.f);
                        break;
                case SDLK_s:
                        newp = rel_p(MOVE_BY, PI);
                        break;
                case SDLK_a:
                        newp = rel_p(MOVE_BY, PI / 2.f);
                        break;
                case SDLK_d:
                        newp = rel_p(MOVE_BY, PI / -2.f);
                        break;
                case SDLK_e:
                        adj_ang(&viewer.view, -ROTATE_BY);
                        break;
                case SDLK_q:
                        adj_ang(&viewer.view,  ROTATE_BY);
                        break;
                default:
                        // Is this the best thing to do here?
                        return FALSE;
                }
        }

        if (1 == i) {           // Indicates no keys were pressed.
                return FALSE;
        }

        if (rq_move(newp)) {
                viewer.pos = newp;
        }

        set_draw();
        return TRUE;
}

void points_on_line(
        struct int2     p1      ,
        struct int2     p2      ,
        struct float2   exts    ,
        struct int2    *a1      ,
        struct int2    *a2
) {
        struct float2 v = viewer.pos;
        float num, den, lambda;

        adj_ang(&exts.x, viewer.view);
        adj_ang(&exts.y, viewer.view);

        if (p2.x == v.x || p1.x == v.x) {
                v.x += 1.f;
        }


        // For a1:
        if (0.f == p2.x - p1.x) {
                num = p1.x - v.x;
                den = cosf(exts.x);
        } else {
                num = v.y - p1.y - (v.x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
                den = sinf(exts.x) + cosf(exts.x) * (p2.y - p1.y) / (p2.x - p1.x);
        }

        lambda = num / den;

        a1->x = v.x + lambda * cosf(exts.x);
        a1->y = v.y - lambda * sinf(exts.x);


        // For a2:
        if (0.f == p2.x - p1.x) {
                num = p1.x - v.x;
                den = cosf(exts.y);
        } else {
                num = v.y - p1.y - (v.x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
                den = sinf(exts.y) + cosf(exts.y) * (p2.y - p1.y) / (p2.x - p1.x);
        }

        lambda = num / den;

// Calculate other coefficient instead and use equation p1 + c (p2 - p1)
        a2->x = v.x + lambda * cosf(exts.y);
        a2->y = v.y - lambda * sinf(exts.y);
}

// Special case of above function for intersection between a view ray and the
// view line (line through centre of view plane).
float l_on_vl(
        float           ang
) {
        struct float2 v = viewer.pos;
        struct float2 p1 = rel_p(FOCAL_L,  HFOV);      // Right way around?
        struct float2 p2 = rel_p(FOCAL_L, -HFOV);
        float num, den;

        adj_ang(&ang, viewer.view);
        float tana = tanf(ang);

        num = v.y - p1.y - tana * (p1.x - v.x);
        den = p2.y - p1.y + tana * (p2.x - p1.x);

        return num / den;
}
