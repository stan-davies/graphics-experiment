#include "gm_man.h"

#include "rend/rend.h"



// Specific wonky rectangle business.



struct gm {
        int             gid     ;

        SDL_Vertex     *vrts    ;

        struct gm      *next    ;
};

#define VRTS_N  4
#define IND_N   6
static int *indices;

static struct {
        struct gm       root    ;
        struct gm      *head    ;

        int             id_n    ;

        int             err     ;
} gm_man;

static struct gm * find_gm(
        int             gid
);

void init_gm_man(
        void
) {
        struct gm root = {
                .gid    = 0     ,
                .next   = NULL
        };
        
        gm_man.root = root;
        gm_man.head = &gm_man.root;

        gm_man.id_n = 1;

        gm_man.err  = GMERR_NONE;

        indices = calloc(IND_N, sizeof(int));
        for (int i = 0; i < IND_N; ++i) {
                indices[i] = 3 - (i >= 3 ? i - 3 : 3 - i);
        }
}

void dest_gm_man(
        void
) {
        struct gm *gmc = gm_man.root.next;
        struct gm *gmn;

        for (;;) {
                if (NULL == gmc) {
                        break;
                }

                gmn = gmc->next;
                free(gmc);
                gmc = gmn;
        }

        free(indices);
}

int gm_man_poll_err(
        void
) {
        return gm_man.err;
}

int create_gm(
        float          *vrt_dat ,
        SDL_Color       col
) {
        SDL_Vertex *vrts = calloc(VRTS_N, sizeof(SDL_Vertex));

        for (int i = 0; i < VRTS_N * 2; i += 2) {
                vrts[i].position.x = vrt_dat[i];
                vrts[i].position.y = vrt_dat[i + 1];
                vrts[i].color = col;

                log_msg("(%.1f, %.1f)", vrts[i].position.x, vrts[i].position.y);
        }

        gm_man.head->next = calloc(1, sizeof(struct gm));

        gm_man.head->next->gid  = gm_man.id_n   ;
        gm_man.head->next->vrts = vrts          ;
        gm_man.head->next->next = NULL          ;

        gm_man.head = gm_man.head->next;

        return gm_man.id_n++;
}

void gm_repos(
        int             gid     ,
        // How do we reposition it? With perspective!?
        int             x       ,
        int             y
) {
        struct gm *sub = find_gm(gid);

        if (!sub) {
                gm_man.err = GMERR_REPOS_NF;
                log_err("Failed to reposition geometry.");
                return;
        }

        x += y - x;
        // ^ So compiler stops getting stroppy about unused variables.

        // Reposition somehow.
}

void draw_gm(
        int             gid
) {
        struct gm *sub = find_gm(gid);

        if (!sub) {
                gm_man.err = GMERR_DRAW_NF;
                log_err("Failed to draw geometry.");
                return;
        }

        rend_gm(sub->vrts, VRTS_N, indices, IND_N);
}

static struct gm * find_gm(
        int             gid
) {
        struct gm *gmc = &gm_man.root;
        for (;;) {
                gmc = gmc->next;

                if (NULL == gmc) {
                        return NULL;
                }

                if (gid == gmc->gid) {
                        return gmc;
                }
        }
}
