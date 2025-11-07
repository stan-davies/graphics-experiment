#include "viewer/viewer.h"

#include <math.h>

#include "rend/rend.h"
#include "key_man/key_man.h"

#define PI              3.141598f
#define FOV             PI / 2.f
#define NOT_VIS         -8.f

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)

static struct {
        struct int2     pos     ;

        float           view    ;       // Angle of view offset from (0, 1), in
                                        // (-pi, pi].

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
        float ang_1 = fabsf(rel_ang(v1));
        if (ang_1 <= FOV / 2) {
                return TRUE;
        }

        float ang_2 = fabsf(rel_ang(v2));
        if (ang_2 <= FOV / 2) {
                return TRUE;
        }

        return FALSE;
}

void calc_ext(
        struct int2     v1      ,
        struct int2     v2      ,
        float          *lm_e    ,       // Leftmost extent.
        float          *rm_e    ,       // Rightmost extent.
        float          *i_e             // Inbetween extent.
) {
        float ang1 = rel_ang(v1);
        float ang2 = rel_ang(v2);
        float in, out;

        if (0.f == ang1 || 0.f == ang2) {
                goto opp;
        } else if (ang1 / ang2 < 0.f) {
                *lm_e =   MIN(fabsf(MIN(ang1, ang2)), FOV / 2.f);
                *rm_e = - MIN(fabsf(MAX(ang1, ang2)), FOV / 2.f);
                *i_e  = *lm_e - *rm_e;
        } else {
opp:
                in  = MIN( MIN( fabsf(ang1), fabsf(ang2) ), FOV / 2.f );
                out = MIN( MAX( fabsf(ang1), fabsf(ang2) ), FOV / 2.f );

                // Ensure we get it right if either angle is 0.
                if (ang1 < 0.f || ang2 < 0.f) {
                        *lm_e = -in;
                        *rm_e = -out;
                } else {
                        *lm_e = out;
                        *rm_e = in;
                }
                
                *i_e = out - in;
        }
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
                        if (viewer.pos.y > 10) {
                                viewer.pos.y -= 10;
                        }
                        break;
                case SDLK_s:
                        if (viewer.pos.y < SCREEN_H - 10) {
                                viewer.pos.y += 10;
                        }
                        break;
                case SDLK_a:
                        if (viewer.pos.x > 10) {
                                viewer.pos.x -= 10;
                        }
                        break;
                case SDLK_d:
                        if (viewer.pos.x < SCREEN_W - 10) {
                                viewer.pos.x += 10;
                        }
                        break;
                case SDLK_e:
                        adj_ang(&viewer.view, PI / -45.f);
                        break;
                case SDLK_q:
                        adj_ang(&viewer.view, PI / 45.f);
                        break;
                default:
                        return FALSE;
                }
        }

        set_draw();
        return TRUE;
}
