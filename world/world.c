#include "world.h"

#include "viewer/viewer.h"
#include "rend/rend.h"
#include "occi_man/occi_man.h"

#define NODE_C          8

#define NO_HELD        -1

#define FAR             1000.f

#define MODE_2D         2
#define MODE_3D         3

#define PI              3.141598f
#define TO_DEG(a) (a * 180.f / PI)

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)

struct wall {
        struct int2     edge    ;

        struct float2   angs    ;       // Relative (to viewer) angle to each
                                        // vertex of wall.
        struct float2   extent  ;       // x = left most, y = right most, both
                                        // in radians relative to viewer,
                                        // bounded by FOV
        float           ext_i   ;       // Interval of extent.
                                        // Probably worth not storing this.

        float           dist    ;       // Current nearest wall's distance (?).

        int             ckd     ;       // Already analysed on current
                                        // rendering pass.
};

static struct {
        struct int2    *verts   ;

        struct wall    *walls   ;

        int             v_held  ;
        int             mode    ;
} world;

static SDL_Color vis_c  = { 30, 240, 45, 255 };
static SDL_Color held_c = { 60, 90, 240, 255 };

static void draw_seg_2d(
        int             w_ind   ,
        struct float2   seg
);

static void draw_seg_3d(
        void
);

static int find_nr_w(
        float          *fr_than
);

void init_world(
        void
) {
        world.verts = calloc(NODE_C, sizeof(struct int2));
        world.walls = calloc(NODE_C, sizeof(struct wall));

        world.v_held = NO_HELD;

        for (int i = 0; i < NODE_C / 2; ++i) {
                world.verts[i].x = (0 == i || 3 == i) ? 100 : SCREEN_W - 100;
                world.verts[i].y = (i >= 2) ? SCREEN_H - 100 : 100;

                world.walls[i].edge.x = i;
                world.walls[i].edge.y = (i + 1) % 4;
        }

        int ofi;
        for (int i = NODE_C / 2; i < NODE_C; ++i) {
                ofi = i % (NODE_C / 2);
                world.verts[i].x = (0 == ofi || 3 == ofi) ? 400 : 500;
                world.verts[i].y = (ofi >= 2) ? 300 : 200;

                world.walls[i].edge.x = ofi + 4;
                world.walls[i].edge.y = (ofi + 1) % 4 + 4;
        }

        update_world();
}

void dest_world(
        void
) {
        free(world.verts);
        world.verts = NULL;

        free(world.walls);
        world.walls = NULL;
}

void update_world(
        void
) {
        SDL_Keycode k;
        int i = 0;

        while (-1 != (k = get_key(i++))) {
                switch (k) {
                case SDLK_3:
                        world.mode = MODE_3D;
                        break;
                case SDLK_2:
                        world.mode = MODE_2D;
                        break;
                default:
                        break;
                }
        }

        for (int w = 0; w < NODE_C; ++w) {      // Iterating over walls.
                if (
                        world.v_held == world.walls[w].edge.x
                     || world.v_held == world.walls[w].edge.y
                     || (       0.f == world.walls[w].ext_i
                             && !vx_in_view(
                                        world.verts[world.walls[w].edge.x],
                                        world.verts[world.walls[w].edge.y]
                                )
                        )
                ) {
                        continue;
                }

                calc_ext(
                        world.verts[world.walls[w].edge.x],
                        world.verts[world.walls[w].edge.y],
                        &world.walls[w].extent,
                        &world.walls[w].angs,
                        &world.walls[w].ext_i
                );

                world.walls[w].dist = calc_nrst(
                        world.verts[world.walls[w].edge.x],
                        world.verts[world.walls[w].edge.y]
                );
        }
}

void check_mclick(
        SDL_MouseButtonEvent    m
) {
        if (NO_HELD != world.v_held) {
                world.v_held = NO_HELD;
                update_world();
                return;
        }

        struct int2 v;
        for (int i = 0; i < NODE_C; ++i) {
                v = world.verts[i];
                if (v.x - 10 < m.x && v.x + 10 > m.x
                 && v.y - 10 < m.y && v.y + 10 > m.y) {
                        world.v_held = i;
                        return;
                }
        }
}

void check_mmove(
        SDL_MouseMotionEvent    m
) {
        if (NO_HELD == world.v_held) {
                return;
        }

        world.verts[world.v_held].x = m.x;
        world.verts[world.v_held].y = m.y;
}

static void draw_seg_2d(
        int             w_ind   ,
        struct float2   seg
) {
        struct int2 a1, a2;
        SDL_Color col = w_ind == world.v_held ? held_c : vis_c;

        points_on_line(
                world.verts[world.walls[w_ind].edge.x],
                world.verts[world.walls[w_ind].edge.y],
                seg, &a1, &a2
        );

        rend_ln(a1, a2, col);
}

static void draw_seg_3d(
        int             w_ind   ,
        struct float2   seg
) {
        struct int2 a1, a2;

        points_on_line(
                world.verts[world.walls[w_ind].edge.x],
                world.verts[world.walls[w_ind].edge.y],
                seg, &a1, &a2
        );

                        // Room for shortcut here?
        float d1 = calc_dist(a1);
        float d2 = calc_dist(a2);

        float h1 = 100.f / d1;
        float h2 = 100.f / d2;
        
        SDL_Vertex verts = calloc(4, sizeof(SDL_Vertex));


        float angc;
        float hc;

        for (int v = 0; v < 4; ++v) {

                if (v % 3 == 0) {
                        angc = seg.x;
                        hc = h1;
                } else {
                        angc = seg.y;
                        hc = h2;
                }

                verts[v].position.x = ang_across_view(angc);

                // Same calculation for all x-coords -> function of viewer.
                /* (ang + -FOV/2 ) / FOV * SCREEN_W */


                verts[v].position.y = /* line of sight */ + hc / 2.f * (v > 1 ? 1 : -1);
                /* Need to go clockwise so first two y-coords are 
                 *    line of sight - h / 2
                 * then latter two are
                 *    line of sight + h / 2
                 * and h is h1 for first and last, h2 for second and third.
                 */

                verts[v].col = /* some kind of map from something arbitrary */
        }

        rend_gm(verts, 4);

        free(verts);
        verts = NULL;
}

void draw_world(
        void
) {
        float nr = 0.f;
        int nr_w;

        struct float2  inter;   // Extent of wall within FOV.
        struct float2 *dr_segs; // Array of segments to draw as angle pairs.
        int drc;

        for (int w = 0; w < NODE_C; ++w) {
                world.walls[w].ckd = FALSE;
        }

        init_occi_man();

        while (-1 != (nr_w = find_nr_w(&nr))) {

                inter.x = MAX(world.walls[nr_w].extent.x, world.walls[nr_w].extent.y);
                inter.y = MIN(world.walls[nr_w].extent.x, world.walls[nr_w].extent.y);

                drc = get_seg(inter, &dr_segs);

                for (int s = 0; s < drc; ++s) {
                        if (MODE_2D == world.mode) {
                                draw_seg_2d(nr_w, dr_segs[s]);
                        } else {
                                draw_seg_3d(nr_w, dr_segs[s]);
                        }
                }

                free(dr_segs);
                dr_segs = NULL;

                // Make it 3D =o  :
                // For each wall, construct four vertices then send to
                // `rend_gm`, needn't bother with `gm_man` really since it will
                // all be recreated each frame =/
        }

        dest_occi_man();
}

static int find_nr_w(
        float          *fr_than
) {
        int nearest = -1;
        float nr_than = FAR;
        float d;
        for (int i = 0; i < NODE_C; ++i) {
                if (0 == world.walls[i].ext_i || world.walls[i].ckd) {
                        continue;
                }

                d = world.walls[i].dist;

                int con_1 = (d < nr_than);
                int con_2 = (d >= *fr_than);
                
                if (con_1 && con_2) {
                        nearest = i;
                        nr_than = d;
                }
        }

        *fr_than = nr_than;
        world.walls[nearest].ckd = TRUE;
        return nearest;
}        
