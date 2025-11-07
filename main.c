#include <stdio.h>

#include "util/util.h"
#include "loop/loop.h"

int main(
        void
) {
        if (!init()) {
                return 0;       // All required cleanup done internally.
        }

        loop();

        end();
        return 0;
}
