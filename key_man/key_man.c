#include "key_man.h"

#define MAX_KEYS        8

static struct {
        SDL_Keycode    *keys    ;
        int             num_d   ;
} key_man;

void init_key_man(
        void
) {
        key_man.keys = calloc(MAX_KEYS, sizeof(SDL_Keycode));
        key_man.num_d = 0;
}

void dest_key_man(
        void
) {
        free(key_man.keys);
        key_man.keys = NULL;
}

void k_down(
        SDL_Keycode     k
) {
        for (int i = 0; i < key_man.num_d; ++i) {
                if (k == key_man.keys[i]) {
                        return;
                }
        }

        if (MAX_KEYS == key_man.num_d) {
                log_err("Too many keys pressed!");
                return;
        }

        key_man.keys[key_man.num_d++] = k;
}

void k_up(
        SDL_Keycode     k
) {
        for (int i = 0; i < key_man.num_d; ++i) {
                if (k == key_man.keys[i]) {
                        key_man.keys[i] = key_man.keys[--key_man.num_d];
                        key_man.keys[key_man.num_d] = 0;
                }
        }
}

SDL_Keycode get_key(
        int             i
) {
        if (i >= key_man.num_d || i < 0) {
                return -1;
        }

        return key_man.keys[i];
}
        
