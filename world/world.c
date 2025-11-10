#include "world.h"

#include "viewer/viewer.h"
#include "rend/rend.h"

#define NODE_C          4

#define NO_HELD        -1
#define HOLD_NEW       -2

#define FAR             1000.f
#define MAX_INTS        8

#define PI              3.141598f
#define TO_DEG(a) (a * 180.f / PI)

#define IN_INTERVAL(ang, interval) (ang > interval.x && ang < interval.y)

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

        float           dist    ;

        int             drawn   ;
};

static struct {
        struct int2    *verts   ;

        struct wall    *walls   ;

        int             v_held  ;
} world;

static SDL_Color vis_c  = { 0, 255, 0, 255 };
static SDL_Color hid_c  = { 255, 0, 0, 255 };
static SDL_Color held_c = { 60, 90, 240, 255 };
static SDL_Color drop_c = { 255, 255, 255, 255 };

static int find_nr_w(
        float          *fr_than
);

static struct float2 interp_angs(
        int             w
);

void init_world(
        void
) {
        world.verts = calloc(NODE_C, sizeof(struct int2));
        world.walls = calloc(NODE_C, sizeof(struct wall));

        world.v_held = NO_HELD;

        for (int i = 0; i < NODE_C; ++i) {
                world.verts[i].x = (0 == i || 3 == i) ? 100 : SCREEN_W - 100;
                world.verts[i].y = (i >= 2) ? SCREEN_H - 100 : 100;

                world.walls[i].edge.x = i;
                world.walls[i].edge.y = (i + 1) % 4;
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
        for (int i = 0; i < NODE_C; ++i) {
                if (
                        i == world.v_held || (
                                0.f == world.walls[i].ext_i
                             && !vx_in_view(
                                        world.verts[world.walls[i].edge.x],
                                        world.verts[world.walls[i].edge.y]
                                )
                        )
                ) {
                        continue;
                }

                calc_ext(
                        world.verts[world.walls[i].edge.x],
                        world.verts[world.walls[i].edge.y],
                        &world.walls[i].extent,
                        &world.walls[i].angs,
                        &world.walls[i].ext_i
                );

                world.walls[i].dist = calc_nrst(
                        world.verts[world.walls[i].edge.x],
                        world.verts[world.walls[i].edge.y]
                );
        }
}

void check_mclick(
        SDL_MouseButtonEvent    m
) {
        if (HOLD_NEW == world.v_held) {
                // check if clicked on an old vertex, if so use that
                // otherwise create new vertex
                // link to previous
                // remain in 'HOLD_NEW'
        } else if (NO_HELD != world.v_held) {
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

        // create new vertex
        world.v_held = HOLD_NEW;
}

void check_mmove(
        SDL_MouseMotionEvent    m
) {
        if (NO_HELD == world.v_held || HOLD_NEW == world.v_held) {
                return;
        }

        world.verts[world.v_held].x = m.x;
        world.verts[world.v_held].y = m.y;
}

void draw_world(
        void
) {
        SDL_Color use;

        for (int i = 0; i < NODE_C; ++i) {
                if (world.v_held == world.walls[i].edge.x
                 || world.v_held == world.walls[i].edge.y) {
                        use = held_c;
//                } else if (world.walls[i].ext_i > 0.f) {
//                        use = vis_c;
                } else {
                        use = hid_c;
                }

                rend_ln(
                        world.verts[world.walls[i].edge.x],
                        world.verts[world.walls[i].edge.y],
                        use
                );
        }
}

void draw_world_3d(
        void
) {
        // Make own function or something?
        // Would probably need multiple =(
//        struct float2 *occ_int = calloc(MAX_INTS, sizeof(struct float2));
//        int ints = 0;

        float nr = 0.f;

        int nr_i;
//        struct float2 occ;
//        int cov;

        int checked = 0;

        // Have not yet devised a better way to do this =(
        for (int i = 0; i < NODE_C; ++i) {
                world.walls[i].drawn = FALSE;
        }

//        while (!spans_fov(occ_int[0])) {
        while (checked < NODE_C) {
                nr_i = find_nr_w(&nr);

                struct float2 lambdas = interp_angs(nr_i);

                struct int2 p_l = {
                        world.verts[world.walls[nr_i].edge.x].x + lambdas.x * (world.verts[world.walls[nr_i].edge.y].x - world.verts[world.walls[nr_i].edge.x].x),
                        world.verts[world.walls[nr_i].edge.x].y + lambdas.x * (world.verts[world.walls[nr_i].edge.y].y - world.verts[world.walls[nr_i].edge.x].y)
                };

                struct int2 p_r = {
                        world.verts[world.walls[nr_i].edge.x].x + lambdas.y * (world.verts[world.walls[nr_i].edge.y].x - world.verts[world.walls[nr_i].edge.x].x),
                        world.verts[world.walls[nr_i].edge.x].y + lambdas.y * (world.verts[world.walls[nr_i].edge.y].y - world.verts[world.walls[nr_i].edge.x].y)
                };

                rend_ln(p_l, p_r, vis_c);

                checked++;


                continue;

// All this interval stuff that I am unclear if I will need.
//                occ = world.walls[nr_i].extent;
//                cov = FALSE;
//
//                for (int i = 0; i < ints; ++i) {
//                        // Left most angle hidden, start from right of nearer
//                        // wall.
//                        if (IN_INTERVAL(occ.x, occ_int[i])) {
//                                occ.x = occ_int[i].y;
//                                cov = TRUE;
//
//                                if (!IN_INTERVAL(occ.y, occ_int[i])) {
//                                        occ_int[i].y = occ.y;
//                                }
//                                // So long as right most angle is visible,
//                                // extend this interval to that angle, but
//                                // when?
//                        }
//
//                        // Right most angle hidden, start from left of nearer
//                        // wall.
//                        if (IN_INTERVAL(occ.y, occ_int[i])) {
//                                occ.y = occ_int[i].x;
//                                cov = TRUE;
//
//                                if (!IN_INTERVAL(occ.x, occ_int[i])) {
//                                        occ_int[i].x = occ.y;
//                                }
//                                // So long as left most angle is visible,
//                                // extend this interval to that angle, but
//                                // when?
//                        }
//                }
//
//                // 0 span or left most angle has moved to right of right most
//                // angle.
//                if (occ.x == occ.y || occ.x > occ.y) {
//                        // Totally hidden!
//                        continue;
//                }
//
//                if (MAX_INTS == ints) {
//                        log_err("Too few intervals.");
//                        break;
//                }
//
//                if (!cov) {
//                        occ_int[ints++] = occ;
//                }
//
//                float lambda_l = ang_interp(
//                        world.verts[world.walls[nr_i].edge.x],
//                        world.verts[world.walls[nr_i].edge.y],
//                        world.walls[nr_i].extent.x
//                );
//
//                float lambda_r = ang_interp(
//                        world.verts[world.walls[nr_i].edge.x],
//                        world.verts[world.walls[nr_i].edge.y],
//                        world.walls[nr_i].extent.y
//                );
//
//                struct int2 p_l = {
//                        world.verts[world.walls[nr_i].edge.x].x + lambda_l * (world.verts[world.walls[nr_i].edge.y].x - world.verts[world.walls[nr_i].edge.x].x),
//                        world.verts[world.walls[nr_i].edge.y].x + lambda_l * (world.verts[world.walls[nr_i].edge.y].y - world.verts[world.walls[nr_i].edge.y].x)
//                };
//
//                struct int2 p_r = {
//                        world.verts[world.walls[nr_i].edge.x].x + lambda_r * (world.verts[world.walls[nr_i].edge.y].x - world.verts[world.walls[nr_i].edge.x].x),
//                        world.verts[world.walls[nr_i].edge.y].x + lambda_r * (world.verts[world.walls[nr_i].edge.y].y - world.verts[world.walls[nr_i].edge.y].x)
//                };
//
//                rend_ln(p_l, p_r, vis_c);
//
//                // Draw wall within this interval - do in 2D for now as reference.
//                // 1. Find the coordinates of the visible ends of the wall.
//                // 2. Draw line between them.
//                // 3D:
//                //  2. Find distance to each point.
//                //  3. Use some kind of fancy linear function to map distance
//                //  to point onto wall height.
//                //  4. Create a floor and ceiling vertex for each point.
//                //  5. Draw up some triangles inbetween.
//
//                // Will we use gm man? Seems kind of silly to use a linked list
//                // given we will just destroy this data and create it all anew
//                // for the next frame =/
//                // Speaking of, is that really the way to go? I guess it is
//                // actually not that complex on the calculations, so yes...
//
//                // Check through intervals, if any overlap, merge into one.
//                for (int i = 0; i < ints; ++i) {
//                        for (int j = 0; j < ints; ++j) {
//                                if (i == j) {
//                                        continue;
//                                }
//
//                                if (IN_INTERVAL(occ_int[i].x, occ_int[j])) {
//                                        occ_int[i].x = occ_int[j].x;
//                                        occ_int[i].y = MAX(occ_int[i].y, occ_int[j].y);
//
//                                        occ_int[j] = occ_int[--ints];
//                                }
//
//                                if (IN_INTERVAL(occ_int[i].y, occ_int[j])) {
//                                        occ_int[i].y = occ_int[j].y;
//                                        occ_int[i].x = MIN(occ_int[i].x, occ_int[j].x);
//
//                                        occ_int[j] = occ_int[--ints];
//                                }
//                        }
//                }
        }
}

static int find_nr_w(
        float          *fr_than
) {
        int nearest;
        float nr_than = FAR;
        float d;
        for (int i = 0; i < NODE_C; ++i) {
                if (0 == world.walls[i].ext_i || world.walls[i].drawn) {
                        continue;
                }

                d = world.walls[i].dist;
                if (d < nr_than && d >= *fr_than) {
                        nearest = i;
                        nr_than = d;
                }
        }

        *fr_than = d;
        // Is this the place to do this?
        world.walls[nearest].drawn = TRUE;
        return nearest;
}        

static struct float2 interp_angs(
        int             w
) {
        struct float2 angs = world.walls[w].angs;

// Linear interpolation is not quite right =/
        struct float2 lambdas = {
                .x = (world.walls[w].extent.x - angs.x) / (angs.y - angs.x),
                .y = (world.walls[w].extent.y - angs.x) / (angs.y - angs.x)
        };

//// Something majorly amiss in here.
//        struct float2 line = {
//                .x = world.verts[world.walls[w].edge.y].x - world.verts[world.walls[w].edge.x].x,
//                .y = world.verts[world.walls[w].edge.y].y - world.verts[world.walls[w].edge.x].y
//        };
//        float tot_w = sqrtf(line.x * line.x + line.y * line.y);
//
//        float d_l = calc_dist(world.verts[world.walls[w].edge.x]);
//        float d_r = calc_dist(world.verts[world.walls[w].edge.y]);
//
//        float ex     = world.walls[w].ext_i;
//        float sin_ie = sinf(PI - ex);
//        float cos_ie = cosf(PI - ex);
//        float thet_l = atanf( sin_ie / (d_l - d_r * cos_ie) ); 
//        float thet_r = atanf( sin_ie / (d_r - d_l * cos_ie) ); 
//
//        float dif_l  = MAX(world.walls[w].extent.x, angs.x) - MIN(world.walls[w].extent.x, angs.x);
//        float dif_r  = MAX(world.walls[w].extent.y, angs.y) - MIN(world.walls[w].extent.y, angs.y);
//
//        float w_l = d_l * sinf(dif_l) / sinf(PI - ex - thet_l);
//        float w_r = d_r * sinf(dif_r) / sinf(PI - ex - thet_r);
//
//        struct float2 lambdas = {
//                .x = w_l / tot_w,
//                .y = w_r / tot_w
//        };

        return lambdas;
}
