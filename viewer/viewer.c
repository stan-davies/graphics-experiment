#include "viewer/viewer.h"

#include <math.h>

#include "rend/rend.h"

#define PI              3.141598f
#define FOV             PI / 2.f
#define NOT_VIS         -8.f
#define SIDE_ANG        PI * 3.f / 4.f

static struct {
        struct int2     pos     ;

        float           view    ;       // Angle of view offset from (0, 1), in
                                        // (-pi, pi].

        struct int2     lookat  ;
        SDL_Rect        body    ;
        
        struct int2     lp_la   ;
        struct int2     rp_la   ;

        struct int2     lop_la  ;
        struct int2     rop_la  ;
} viewer;

static SDL_Color bod_col = { 30, 45, 240, 255 };
static SDL_Color prong_col = { 255, 90, 120, 255 };
static SDL_Color oprong_col = { 120, 90, 255, 255 };

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

static void set_draw(
        void
) {
        viewer.lookat.x = viewer.pos.x + 50 * cosf(viewer.view);
        viewer.lookat.y = viewer.pos.y - 50 * sinf(viewer.view);

        viewer.body.x = viewer.pos.x - 5;
        viewer.body.y = viewer.pos.y - 5;

        viewer.lp_la.x = viewer.pos.x + 500 * cosf(viewer.view + FOV / 2.f);
        viewer.lp_la.y = viewer.pos.y - 500 * sinf(viewer.view + FOV / 2.f);

        viewer.rp_la.x = viewer.pos.x + 500 * cosf(viewer.view - FOV / 2.f);
        viewer.rp_la.y = viewer.pos.y - 500 * sinf(viewer.view - FOV / 2.f);

        viewer.lop_la.x = viewer.pos.x + 500 * cosf(viewer.view + SIDE_ANG);
        viewer.lop_la.y = viewer.pos.y - 500 * sinf(viewer.view + SIDE_ANG);

        viewer.rop_la.x = viewer.pos.x + 500 * cosf(viewer.view - SIDE_ANG);
        viewer.rop_la.y = viewer.pos.y - 500 * sinf(viewer.view - SIDE_ANG);
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

        return fabsf(v_ang);
}

int visible(
        struct int2     v1      ,
        struct int2     v2
) {
        float ang_1 = rel_ang(v1);
        if (ang_1 <= FOV / 2) {
                return TRUE;
        }

        float ang_2 = rel_ang(v2);
        if (ang_2 <= FOV / 2) {
                return TRUE;
        }

        if (ang_1 <= SIDE_ANG && ang_2 <= SIDE_ANG) {
                return TRUE;
        }
        // ^ Will pick up even the most grazing cases but also allows invisible
        // things to be considered visible.

        return FALSE;
}

void draw_viewer(
        void
) {
        rend_rc(viewer.body, bod_col);
        rend_ln(viewer.pos, viewer.lookat, bod_col);

        rend_ln(viewer.pos, viewer.lp_la, prong_col);
        rend_ln(viewer.pos, viewer.rp_la, prong_col);

        rend_ln(viewer.pos, viewer.lop_la, oprong_col);
        rend_ln(viewer.pos, viewer.rop_la, oprong_col);
}

void update_viewer(
        SDL_KeyboardEvent       k
) {
        switch (k.keysym.sym) {
        case SDLK_w:
                viewer.pos.y -= 10;
                break;
        case SDLK_s:
                viewer.pos.y += 10;
                break;
        case SDLK_a:
                viewer.pos.x -= 10;
                break;
        case SDLK_d:
                viewer.pos.x += 10;
                break;
        case SDLK_e:
                adj_ang(&viewer.view, PI / -45.f);
                break;
        case SDLK_q:
                adj_ang(&viewer.view, PI / 45.f);
                break;
        default:
                return;
        }

        set_draw();
}
