#include "world.h"

#include "viewer/viewer.h"
#include "rend/rend.h"

#define VXS_C   4
#define EDG_C   4

static struct {
        struct int2    *verts   ;
        struct int2    *edges   ;
} world;

static SDL_Color vis = { 0, 255, 0, 255 };
static SDL_Color hid = { 255, 0, 0, 255 };

void init_world(
        void
) {
        world.verts = calloc(VXS_C, sizeof(struct int2));
        world.edges = calloc(EDG_C, sizeof(struct int2));

        for (int i = 0; i < 4; ++i) {
                world.verts[i].x = (0 == i || 3 == i) ? 100 : SCREEN_W - 100;
                world.verts[i].y = (i >= 2) ? SCREEN_H - 100 : 100;

                world.edges[i].x = i;
                world.edges[i].y = (i + 1) % 4;
        }

        struct int2 edge;

        for (int i = 0; i < EDG_C; ++i) {
                edge = world.edges[i];
                log_msg("i: %d, e: (%d, %d), v1: (%d, %d), v2: (%d, %d)",
                        i, edge.x, edge.y,
                        world.verts[edge.x].x, world.verts[edge.x].y,
                        world.verts[edge.y].x, world.verts[edge.y].y
                );
        }
}

void dest_world(
        void
) {
        free(world.verts);
        free(world.edges);
        world.verts = world.edges = NULL;
}

void draw_world(
        void
) {
        SDL_Color use;
        struct int2 edge;

        for (int i = 0; i < EDG_C; ++i) {
                edge = world.edges[i];
                if (visible(world.verts[edge.x], world.verts[edge.y])) {
                        use = vis;
                } else {
                        use = hid;
                }

                rend_ln(world.verts[edge.x], world.verts[edge.y], use);
        }
}
