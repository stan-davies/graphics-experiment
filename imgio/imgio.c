#include "imgio.h"

#include <stdio.h>
#include <stdlib.h>

#define DIM_LNN 1
#define DAT_BEG 3

int read_img(
        char           *path    , 
        struct img     *read
) {
        // path relative to directory instead?
        FILE *f = fopen(path, "r");
        if (!f) {
                return FALSE;
        }

        // this seems bloated but also I think it is a bit naff error-handling
        // wise since it supposes correct formatting
        size_t        f_len = 0;
        char         *ln    = NULL;
        ssize_t       ln_len;
        struct col   *px;
        int           p     = -1;
        int           dimf  = FALSE;

        while ((ln_len = getline(&ln, &f_len, f)) != -1) {
                ++p;
                if (DIM_LNN == p) {
                        sscanf(ln, "%d %d", &(read->w), &(read->h));
                        read->dat = calloc(read->w * read->h, 
                                        sizeof(struct col));
                        dimf = TRUE;
                } else if (p < DAT_BEG) {
                        continue;
                } else if (p - DAT_BEG > read->w * read->h) {
                        break;
                } else if (dimf) {
                        px = &(read->dat[p - DAT_BEG]);
                        sscanf(ln, "%c %c %c", &(px->r), &(px->g), &(px->b));
                } else {
                        printf("problem\n");
                }
        }

        fclose(f);
        free(ln);
        return TRUE;
}
