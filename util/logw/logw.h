#ifndef LOGW_H
#define LOGW_H

int init_log(
        void
);

void log_msg(
        char           *fmt     ,
        ...
);

void log_err(
        char           *fmt     ,
        ...
);

#endif
