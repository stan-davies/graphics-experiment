#include "logw.h"

#include <stdio.h>
#include <stdarg.h>

#define LOGF_PATH "logf"

// Because of wanting this to be included easily everywhere, this is included
// from the same file in which I defined true and false. To avoid any confusing
// include chains, I have just returned values '0' and '1' in `init_log`.

int init_log(
        void
) {
        FILE *f = fopen(LOGF_PATH, "w");
        if (!f) { 
                printf("Error opening log file. How did it come to this?\n");
                return 0;
        }

        // Purpose of this is to overwrite log file from previous run.
        fprintf(f, "");
        
        fclose(f);
        return 1;
}

void log_msg(
        char           *fmt     ,
        ...
) {
        FILE *f = fopen(LOGF_PATH, "a");
        if (!f) {
                printf("Error opening log file. How did it come to this?\n");
                return;
        }

        va_list argptr;
        va_start(argptr, fmt);

        vfprintf(f, fmt, argptr);
        fprintf(f, "\n");

        va_end(argptr);

        fclose(f);
}

void log_err(
        char           *fmt     ,
        ...
) {
        FILE *f = fopen(LOGF_PATH, "a");
        if (!f) {
                printf("Error opening log file. How did it come to this?\n");
                return;
        }

        va_list argptr;
        va_start(argptr, fmt);

        fprintf(f, "Error: ");
        vfprintf(f, fmt, argptr);
        fprintf(f, "\n");

        va_end(argptr);

        fclose(f);
}
