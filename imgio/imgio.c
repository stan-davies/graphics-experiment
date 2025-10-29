#include "imgio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        int           n, r, g, b;       // For reading pixel data.
                                        // - Number of channels read.
                                        // - Raw data for r/g/b channels.

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

                        if (0 == strcmp("a \n", ln)) {
                                px->a = px->r = px->g = px->b = 0;
                                continue;
                        }

                        n = sscanf(ln, "%d %d %d", &r, &g, &b);
                        // Cast from int to Uint8 force into new limits, so if
                        // they are the same then the original numbers were
                        // valid.
                        px->r = r;
                        px->g = g;
                        px->b = b;
                        px->a = 255;

                        if (3 != n || px->r != r || px->g != g || px->b != b) {
                                printf("Error: Invalid pixel data.\n");

                                dimf = FALSE;
                                goto err_f;
                        }
                } else {
err_f:
                        // Musn't be done pre-allocation.
                        free(read->dat);
err:
                        // Dimensions were invalid or tried to read pixel
                        // without knowing dimensions. Either way, `dimf` is
                        // false. In all other cases, `dimf` is true. However,
                        // we do just have to set `dimf` to be false if the
                        // pixel data is invalid.
                        printf("Error: Could not read texture at '%s'.\n", path);
                        goto exit;
                }
        }

        // Only run when `dimf` was true. Ensures number of read pixels is
        // exactly equal to the number of pixels allocated for.
        if (p - DAT_BEG + 1 != read->w * read->h) {
                free(read->dat);
                dimf = FALSE;           // Return error.
        }

exit:
        free(ln);
        fclose(f);
        return dimf;
}
