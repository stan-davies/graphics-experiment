#include "imgio.h"

#include <stdio.h>
#include <stdlib.h>

#define DIM_LNN 1       // Line number where dimensions should be.
#define DAT_BEG 3       // Line number where pixel data begins.

// Note that we ignore the P3/P6 stuff as well as the channel bit depth. We
// assume RGB24 here.

int read_img(
        char           *path    , 
        struct img     *read
) {
        // path relative to directory instead?
        FILE *f = fopen(path, "r");
        if (!f) {
                return FALSE;
        }

        // `getline` will use `realloc` on `ln` if it needs to. 16 is the
        // minimum size for which it does not need to.
        char         *ln    = calloc(16, sizeof(char)); // Current line.
        size_t        f_len = 16;       // Size of above buffer.
        ssize_t       ln_len    ;       // Actual length of current line.

        struct col   *px        ;       // Pixel to write to.
        int           p     = -1;       // Pixel index in img.

        int           dimf  = FALSE;    // Whether dimensions have been found.

        while ((ln_len = getline(&ln, &f_len, f)) != -1) {
                p++;

                if (DIM_LNN == p) {
                        if (2 != sscanf(ln, "%d %d", &(read->w), &(read->h))) {
                                goto err;
                        }

                        read->dat = calloc(read->w * read->h,
                                                         sizeof(struct col));
                        dimf = TRUE;
                } else if (p < DAT_BEG) {
                        continue;
                } else if (p - DAT_BEG > read->w * read->h) {
                        break;
                } else if (dimf) {
                        px = &(read->dat[p - DAT_BEG]);
                        // `hhu` is an unsigned char in decimal representation,
                        // i.e. `Uint8`.
                        sscanf(ln, "%hhu %hhu %hhu", 
                                        &(px->r), &(px->g), &(px->b));
                } else {
err:
                        // Dimensions were invalid or tried to read pixel
                        // without knowing dimensions. Either way, `dimf` is
                        // false. In all other cases, `dimf` is true.
                        printf("Error: Could not read texture at '%s'.\n", path);
                        goto exit;
                }
        }

exit:
        free(ln);
        fclose(f);
        return dimf;
}
