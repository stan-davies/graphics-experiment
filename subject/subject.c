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
        // replace with texture stuff
//        struct col      c       ;

        struct {
                int     x       ;
                int     y       ;
        } pos                   ;

//        int             rid     ;
        int             tid     ;
} sub;

void sub_init(
        void
) {
        sub.pos.x       = HOME_X;
        sub.pos.y       = HOME_Y;

//        sub.c.r         = HOME_R;
//        sub.c.g         = HOME_G;
//        sub.c.b         = HOME_B;

//        sub.rid = create_rc(HOME_W, HOME_H, sub.pos.x, sub.pos.y,
//                                sub.c.r, sub.c.g, sub.c.b);
        sub.tid = create_tx(HOME_W, HOME_H, sub.pos.x, sub.pos.y, 
                                                        "assets/norm.ppm");
}

// destroy - not yet needed

// pass in keys pressed and handle internally
void sub_update(
        SDL_Event       e
) {
        if (e.type != SDL_KEYDOWN) {
                return;
        }

        switch (e.key.keysym.sym) {
//        case SDLK_c:
//                sub.c.r = (sub.c.g - 50) % 255; 
//                sub.c.g = (sub.c.b - 20) % 255;
//                sub.c.b = (sub.c.r + 20) % 255;
//                rc_recol(sub.rid, sub.c.r, sub.c.g, sub.c.b);
//                break;
        case SDLK_w:
                sub.pos.y -= 10;
//                rc_repos(sub.rid, sub.pos.x, sub.pos.y);
                tx_repos(sub.tid, sub.pos.x, sub.pos.y);
                break;
        case SDLK_s:
                sub.pos.y += 10;
//                rc_repos(sub.rid, sub.pos.x, sub.pos.y);
                tx_repos(sub.tid, sub.pos.x, sub.pos.y);
                break;
        default:
                break;
        }
}

void sub_draw(
        void
) {
//        draw_rc(sub.rid);
        draw_tx(sub.tid);
}
