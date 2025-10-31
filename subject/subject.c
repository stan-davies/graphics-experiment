#include "subject.h"

#include "rc_man/rc_man.h"
#include "tx_man/tx_man.h"

#define HOME_X  50
#define HOME_Y  50
#define HOME_W  300
#define HOME_H  300
#define HOME_R  30
#define HOME_G  45
#define HOME_B  240

static struct {
        struct {
                int     x       ;
                int     y       ;
        } pos                   ;

        int             tid     ;
} sub;

int sub_init(
        void
) {
        sub.pos.x       = HOME_X;
        sub.pos.y       = HOME_Y;

        sub.tid         = create_tx(HOME_W, HOME_H, sub.pos.x, sub.pos.y, 
                                                        "assets/norm.ppm");

        return -1 != sub.tid;
}

// destroy - not yet needed

void sub_update(
        SDL_Event       e
) {
        if (e.type != SDL_KEYDOWN) {
                return;
        }

        switch (e.key.keysym.sym) {
        case SDLK_w:
                sub.pos.y -= 10;
                tx_repos(sub.tid, sub.pos.x, sub.pos.y);
                break;
        case SDLK_s:
                sub.pos.y += 10;
                tx_repos(sub.tid, sub.pos.x, sub.pos.y);
                break;
        default:
                break;
        }
}

void sub_draw(
        void
) {
        draw_tx(sub.tid);
}
