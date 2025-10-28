#include "tx_man.h"

#include "rend/rend.h"

// poll err function

struct tx {
        int             tid     ;

        SDL_Rect        bb      ;       // In screen space. I.e. `dstrect`.
        SDL_Texture    *tex     ;

        struct tx      *next    ;
};

static struct {
        struct tx       root    ;
        struct tx      *head    ;

        int             id_n    ;

        int             err     ;
} tx_man;

static struct tx * find_tx(
        int             tid
);

void init_tx_man(
        void
) {
        struct tx root = {
                .tid    = 0     ,
                .next   = NULL
        };
        
        tx_man.root = root;
        tx_man.head = &tx_man.root;

        tx_man.id_n = 1;
}

void dest_tx_man(
        void
) {
        struct tx *txc = tx_man.root.next;
        struct tx *txn;

        for (;;) {
                if (NULL == txc) {
                        break;
                }

                SDL_DestroyTexture(txc->tex);

                txn = txc->next;
                free(txc);
                txc = txn;
        }
}

int create_tx(
        int             w       ,
        int             h       ,
        int             x       ,
        int             y       ,
        char           *tx_path
) {
        SDL_Rect        bb      = { x, y, w, h };
        SDL_Texture    *tex     = rend_rq_tex(SDL_PIXELFORMAT_RGB24, 
                                        SDL_TEXTUREACCESS_STATIC, bb.w, bb.h);
        
        struct img i;
        read_img(tx_path, &i);

        // Should pixels be normalised?
        SDL_UpdateTexture(tex, NULL, i.dat, 3 * i.w * sizeof(int));

        free(i.dat);
        // can I free the pixels now? (I think so)

        // ^ possible errors abound (!?)

        tx_man.head->next = calloc(1, sizeof(struct tx));

        tx_man.head->next->tid    = tx_man.id_n ;
        tx_man.head->next->bb     = bb          ;
        tx_man.head->next->tex    = tex         ;
        tx_man.head->next->next   = NULL        ;

        tx_man.head = tx_man.head->next;

        return tx_man.id_n++;
}

void tx_repos(
        int             tid     ,
        int             x       ,
        int             y
) {
        struct tx *sub = find_tx(tid);

        if (!sub) {
                tx_man.err = TXERR_REPOS_NF;
                printf("bad repos\n");
                return;
        }

        sub->bb.x = x;
        sub->bb.y = y;
}

void draw_tx(
        int             tid
) {
        struct tx *sub = find_tx(tid);

        if (!sub) {
                tx_man.err = TXERR_DRAW_NF;
                printf("bad draw\n");
                return;
        }

        rend_tex(&sub->bb, sub->tex);
}

static struct tx * find_tx(
        int             tid
) {
        struct tx *txc = &tx_man.root;
        for (;;) {
                txc = txc->next;

                if (NULL == txc) {
                        return NULL;
                }

                if (tid == txc->tid) {
                        return txc;
                }
        }
}
