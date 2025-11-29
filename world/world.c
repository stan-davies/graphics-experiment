#include "world.h"

#include "viewer/viewer.h"
#include "rend/rend.h"

#define NODE_C          8

#define NO_HELD        -1
#define HOLD_NEW       -2

#define FAR             1000.f
#define MAX_INTS        8

#define PI              3.141598f
#define TO_DEG(a) (a * 180.f / PI)

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)

#define IN_INTERVAL(ang, interval) (ang >= MIN(interval.x, interval.y) && ang <= MAX(interval.x, interval.y))

struct wall {
        struct int2     edge    ;

        struct float2   angs    ;       // Relative (to viewer) angle to each
                                        // vertex of wall.
        struct float2   extent  ;       // x = left most, y = right most, both
                                        // in radians relative to viewer,
                                        // bounded by FOV
        float           ext_i   ;       // Interval of extent.
                                        // Probably worth not storing this.

        float           dist    ;

        int             ckd     ;       // Already analysed on current
                                        // rendering pass.
};

static struct {
        struct int2    *verts   ;

        struct wall    *walls   ;

        int             v_held  ;
} world;

static SDL_Color vis_c  = { 0, 255, 0, 255 };
static SDL_Color hid_c  = { 255, 0, 0, 255 };
static SDL_Color held_c = { 60, 90, 240, 255 };
static SDL_Color drop_c = { 30, 45, 240, 255 };

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
//        world.v_held = HOLD_NEW;
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

        for (int w = 0; w < NODE_C; ++w) {
                if (world.v_held == world.walls[w].edge.x
                 || world.v_held == world.walls[w].edge.y) {
                        use = held_c;
                } else if (world.walls[w].ext_i > 0.f) {
                        use = drop_c;
                } else {
                        use = hid_c;
                }

                rend_ln(
                        world.verts[world.walls[w].edge.x],
                        world.verts[world.walls[w].edge.y],
                        use
                );
        }
}

void draw_world_3d(
        void
) {
        struct float2 *occ_int = calloc(MAX_INTS, sizeof(struct float2));
        int ints = 0;

        float nr = 0.f;

        int nr_w;
        struct float2 occ;

        for (int w = 0; w < NODE_C; ++w) {
                world.walls[w].ckd = FALSE;
        }

        while (-1 != (nr_w = find_nr_w(&nr))) {
                // No idea if this works properly.
                if (spans_fov(occ_int[0])) {
                        break;
                }
                occ.x = MAX(world.walls[nr_w].extent.x, world.walls[nr_w].extent.y);
                occ.y = MIN(world.walls[nr_w].extent.x, world.walls[nr_w].extent.y);

                // What if a wall needs to be split into two?
                // '-> Angles may not be contained, whole thing drawn =(

                for (int i = 0; i < ints; ++i) {
                        // Left most angle hidden, start from right of nearer
                        // wall.
                        if (IN_INTERVAL(occ.x, occ_int[i])) {
                                occ.x = occ_int[i].y;

// Does this make any sense?
                                if (!IN_INTERVAL(occ.y, occ_int[i])) {
                                        occ_int[i].y = occ.y;
                                }
                        }

                        // Right most angle hidden, start from left of nearer
                        // wall.
                        if (IN_INTERVAL(occ.y, occ_int[i])) {
                                occ.y = occ_int[i].x;

                                if (!IN_INTERVAL(occ.x, occ_int[i])) {
                                        occ_int[i].x = occ.y;
                                }
                        }
                }

                if (occ.x <= occ.y) {
                        // Totally hidden!
                        goto clean;
                }

                if (MAX_INTS == ints) {
                        log_err("Too few intervals.");
                        break;
                }

                occ_int[ints++] = occ;

                struct int2 a1, a2;

                points_on_line(
                        world.verts[world.walls[nr_w].edge.x],
                        world.verts[world.walls[nr_w].edge.y],
                        occ, &a1, &a2
                );

                rend_ln(a1, a2, vis_c);

                // Make it 3D =o

clean:
                for (int i = 0; i < ints; ++i) {
                        for (int j = 0; j < ints; ++j) {
                                if (i == j) {
                                        continue;
                                }

// This could conceivably be total rubbish.
                                if (IN_INTERVAL(occ_int[i].x, occ_int[j])) {
                                        occ_int[i].x = occ_int[j].x;
                                        occ_int[i].y = MAX(occ_int[i].y, occ_int[j].y);

                                        occ_int[j] = occ_int[--ints];
                                }

                                if (IN_INTERVAL(occ_int[i].y, occ_int[j])) {
                                        occ_int[i].y = occ_int[j].y;
                                        occ_int[i].x = MIN(occ_int[i].x, occ_int[j].x);

                                        occ_int[j] = occ_int[--ints];
                                }
                        }
                }
        }
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
