#include "subject.h"

#include "util/util.h"
#include "rc_man/rc_man.h"

#define HOME_X  50
#define HOME_Y  50
#define HOME_W  400
#define HOME_H  300
#define HOME_R  30
#define HOME_G  45
#define HOME_B  240

static struct {
        struct col      c       ;

        struct {
                int     x       ;
                int     y       ;
        } pos                   ;

        int             rid     ;
} sub;

void sub_init(
        void
) {
        sub.pos.x       = HOME_X;
        sub.pos.y       = HOME_Y;

        sub.c.r         = HOME_R;
        sub.c.g         = HOME_G;
        sub.c.b         = HOME_B;

        sub.rid = create_rc(HOME_W, HOME_H, sub.pos.x, sub.pos.y,
                                sub.c.r, sub.c.g, sub.c.b);
}

// destroy - not yet needed

// pass in keys pressed and handle internally
void sub_update(
        void
) {
        sub.c.r = (sub.c.g - 50) % 255; 
        sub.c.g = (sub.c.b - 20) % 255;
        sub.c.b = (sub.c.r + 20) % 255;
        rc_recol(sub.rid, sub.c.r, sub.c.g, sub.c.b);
}

void sub_draw(
        void
) {
        draw_rc(sub.rid);
}
