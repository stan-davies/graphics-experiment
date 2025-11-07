#include "world.h"

#include "viewer/viewer.h"
#include "rend/rend.h"

#define NODE_C  4

#define TO_DEG(a) (a * 180.f / 3.141598f)

struct wall {
        struct int2     edge    ;
        struct {
                float   lm      ;
                float   rm      ;
                float   i       ;
        } extent                ;
};

static struct {
        struct int2    *verts   ;

        struct wall    *walls   ;

        int             v_held  ;
} world;

static SDL_Color vis_c  = { 0, 255, 0, 255 };
static SDL_Color hid_c  = { 255, 0, 0, 255 };
static SDL_Color held_c = { 60, 90, 240, 255 };

void init_world(
        void
) {
        world.verts = calloc(NODE_C, sizeof(struct int2));
        world.walls = calloc(NODE_C, sizeof(struct wall));

        world.v_held = 0;

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
                                0.f == world.walls[i].extent.i
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
                        &world.walls[i].extent.lm,
                        &world.walls[i].extent.rm,
                        &world.walls[i].extent.i
                );
        }
}

void draw_world(
        void
) {
        SDL_Color use;

        for (int i = 0; i < NODE_C; ++i) {
                if (world.v_held == world.walls[i].edge.x
                 || world.v_held == world.walls[i].edge.y) {
                        use = held_c;
                } else if (world.walls[i].extent.i > 0.f) {
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
