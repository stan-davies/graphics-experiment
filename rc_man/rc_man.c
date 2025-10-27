#include "rc_man.h"

#include "rend/rend.h"

// poll err function

struct rc {
        int             rid     ;

        SDL_Rect        bb      ;
        struct col      cl      ;

        struct rc      *next    ;
};

static struct {
        struct rc       root    ;
        struct rc      *head    ;

        int             id_n    ;

        int             err     ;
} rc_man;

static struct rc * find_rc(
        int             rid
);

void init_rc_man(
        void
) {
        struct rc root = {
                .rid    = 0     ,
                .next   = NULL
        };
        
        rc_man.root = root;
        rc_man.head = &rc_man.root;

        rc_man.id_n = 1;
}

void dest_rc_man(
        void
) {
        struct rc *rcc = rc_man.root.next;
        struct rc *rcn;

        for (;;) {
                if (NULL == rcc) {
                        break;
                }

                rcn = rcc->next;
                free(rcc);
                rcc = rcn;
        }
}

int create_rc(
        int             w       ,
        int             h       ,
        int             x       ,
        int             y       ,
        Uint8           r       ,
        Uint8           g       ,
        Uint8           b
) {
        SDL_Rect        bb      = { x, y, w, h };
        struct col      cl      = { r, g, b };

        rc_man.head->next = calloc(1, sizeof(struct rc));

        rc_man.head->next->rid    = rc_man.id_n ;
        rc_man.head->next->bb     = bb          ;
        rc_man.head->next->cl     = cl          ;
        rc_man.head->next->next   = NULL        ;

        rc_man.head = rc_man.head->next;

        return rc_man.id_n++;
}

void rc_recol(
        int             rid     ,
        Uint8           r       ,
        Uint8           g       ,
        Uint8           b
) {
        struct rc *sub = find_rc(rid);

        if (!sub) {
                rc_man.err = RCERR_RECOL_NF;
                printf("bad recol\n");
                return;
        }

        sub->cl.r = r;
        sub->cl.g = g;
        sub->cl.b = b;
}

void rc_repos(
        int             rid     ,
        int             x       ,
        int             y
) {
        struct rc *sub = find_rc(rid);

        if (!sub) {
                rc_man.err = RCERR_RECOL_NF;
                printf("bad repos\n");
                return;
        }

        sub->bb.x = x;
        sub->bb.y = y;
}

void draw_rc(
        int             rid
) {
        struct rc *sub = find_rc(rid);

        if (!sub) {
                rc_man.err = RCERR_DRAW_NF;
                printf("bad draw\n");
                return;
        }

        rend_rc(sub->bb, sub->cl);
}

static struct rc * find_rc(
        int             rid
) {
        struct rc *rcc = &rc_man.root;
        for (;;) {
                rcc = rcc->next;

                if (NULL == rcc) {
                        return NULL;
                }

                if (rid == rcc->rid) {
                        return rcc;
                }
        }
}
