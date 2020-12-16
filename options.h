#ifndef PGR_OPTIONS_H
#define PGR_OPTIONS_H

#include <stdbool.h>

    typedef enum t_replacement_strategy {
        RSTRAT_RDM,
        RSTRAT_LRU,
        RSTRAT_FIFO,
        RSTRAT_VMS
    } t_replacement_strategy;

    typedef struct t_options {
        char* file;
        unsigned int nframes;
        t_replacement_strategy replacement_strategy;
        bool quiet_mode;
    } t_options;

    t_options get_options(int argc, char* argv[]);
#endif //PGR_OPTIONS_H
