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

} world;

static SDL_Color vis_c = { 0, 255, 0, 255 };
static SDL_Color hid_c = { 255, 0, 0, 255 };

void init_world(
        void
) {
        world.verts = calloc(NODE_C, sizeof(struct int2));
        world.walls = calloc(NODE_C, sizeof(struct wall));

        for (int i = 0; i < NODE_C; ++i) {
                world.verts[i].x = (0 == i || 3 == i) ? 100 : SCREEN_W - 100;
                world.verts[i].y = (i >= 2) ? SCREEN_H - 100 : 100;

                world.walls[i].edge.x = i;
                world.walls[i].edge.y = (i + 1) % 4;
                
                world.walls[i].extent.lm = 0;
                world.walls[i].extent.rm = 0;
        }
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
                        0.f == world.walls[i].extent.i
                     && !visible(
                                world.verts[world.walls[i].edge.x],
                                world.verts[world.walls[i].edge.y]
                        )
                ) {
                        continue;
                }

                calc_ext(
                        world.verts[world.walls[i].edge.x],
                        world.verts[world.walls[i].edge.y],
                        &(world.walls[i].extent.lm),
                        &(world.walls[i].extent.rm),
                        &(world.walls[i].extent.i)
                );
        }
}

void draw_world(
        void
) {
        SDL_Color use;
        struct wall w;

        update_world();

        for (int i = 0; i < NODE_C; ++i) {
                w = world.walls[i];

                use = w.extent.i > 0.f ? vis_c : hid_c;
                rend_ln(world.verts[w.edge.x], world.verts[w.edge.y], use);
        }
}
