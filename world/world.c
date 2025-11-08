#include "world.h"

#include "viewer/viewer.h"
#include "rend/rend.h"

#define NODE_C          4

#define NO_HELD        -1
#define HOLD_NEW       -2

#define FAR             1000.f
#define MAX_INTS        8

#define TO_DEG(a) (a * 180.f / 3.141598f)

#define IN_INTERVAL(ang, interval) (ang > interval.x && ang < interval.y)

struct wall {
        struct int2     edge    ;

        struct float2   extent  ;       // x = left most, y = right most, both
                                        // in radians relative to viewer,
                                        // bounded by FOV
        float           ext_i   ;       // Interval of extent.

        float           dist    ;
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

static int find_near_w(
        float           nr_than ,
        float           fr_than
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
                        &world.walls[i].extent.x,
                        &world.walls[i].extent.y,
                        &world.walls[i].ext_i
                );

                calc_dist(
                        world.verts[world.walls[i].edge.x],
                        world.verts[world.walls[i].edge.y],
                        &world.walls[i].dist
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
                } else if (world.walls[i].ext_i > 0.f) {
                        use = vis_c;
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
        struct float2 *occ_int = calloc(MAX_INTS, sizeof(struct float2));
        int ints = 0;

        float nr = 0.f;
        float fr = FAR;

        int nr_i;
        struct float2 occ;
        int cov;

        while (!spans_fov(occ_int[0])) {
                nr_i = find_nr_w(fr, nr);
                occ = world.walls[nr_i].extent;
                cov = FALSE;

                for (int = 0; i < ints; ++i) {
                        // Left most angle hidden, start from right of nearer
                        // wall.
                        if (IN_INTERVAL(occ.x, occ_int[i])) {
                                occ.x = occ_int[i].y;
                                cov = TRUE;

                                if (!IN_INTERVAL(occ.y, occ_int[i])) {
                                        occ_int[i].y = occ.y;
                                }
                                // So long as right most angle is visible,
                                // extend this interval to that angle, but
                                // when?
                        }

                        // Right most angle hidden, start from left of nearer
                        // wall.
                        if (IN_INTERVAL(occ.y, occ_int[i])) {
                                occ.y = occ_int[i].x;
                                cov = TRUE;

                                if (!IN_INTERVAL(occ.x, occ_int[i])) {
                                        occ_int[i].x = occ.y;
                                }
                                // So long as left most angle is visible,
                                // extend this interval to that angle, but
                                // when?
                        }
                }

                // 0 span or left most angle has moved to right of right most
                // angle.
                if (occ.x == occ.y || occ.x > occ.y) {
                        // Totally hidden!
                        continue;
                }

                if (MAX_INTS == ints) {
                        log_err("Too few intervals.");
                        break;
                }

                if (!cov) {
                        occ_int[ints++] = occ;
                }

                // Draw wall within this interval - do in 2D for now as reference.
                // 1. Find the coordinates of the visible ends of the wall.
                // 2. Draw line between them.
                // 3D:
                //  2. Find distance to each point.
                //  3. Use some kind of fancy linear function to map distance
                //  to point onto wall height.
                //  4. Create a floor and ceiling vertex for each point.
                //  5. Draw up some triangles inbetween.

                // Will we use gm man? Seems kind of silly to use a linked list
                // given we will just destroy this data and create it all anew
                // for the next frame =/
                // Speaking of, is that really the way to go? I guess it is
                // actually not that complex on the calculations, so yes...

                // Check through intervals, if any overlap, merge into one.
                for (int i = 0; i < ints; ++i) {
                        for (j = 0; j < ints; ++j) {
                                if (i == j) {
                                        continue;
                                }

                                if (IN_INTERVAL(occ_ints[i].x, occ_ints[j])) {
                                        occ_ints[i].x = occ_ints[j].x
                                        occ_ints[i].y = MAX(occ_ints[i].y, occ_ints[j].y);

                                        occ_ints[j] = occ_ints[--ints];
                                }

                                if (IN_INTERVAL(occ_ints[i].y, occ_ints[j])) {
                                        occ_ints[i].y = occ_ints[j].y;
                                        occ_ints[i].x = MIN(occ_ints[i].x, occ_ints[j].x);

                                        occ_ints[j] = occ_ints[--ints];
                                }
                        }
                }
        }
}

static int find_near_w(
        float           nr_than ,
        float           fr_than
) {
        int nearest;
        float d;
        for (int i = 0; i < NODE_C; ++i) {
                d = world.walls[i].dist;
                if (d < nr_than && d > fr_than) {
                        nearest = i;
                        dst = d;
                }
        }

        return nearest;
}        
