#include "options.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_options get_options(int argc, char* argv[]) {
    t_options options;
    options.file= argv[1];
    options.nframes= atoi(argv[2]);

    if(strcmp("rdm", argv[3]) == 0)
        options.replacement_strategy= RSTRAT_RDM;
    else if(strcmp("lru", argv[3]) == 0)
        options.replacement_strategy= RSTRAT_LRU;
    else if(strcmp("fifo", argv[3]) == 0)
        options.replacement_strategy= RSTRAT_FIFO;
    else if(strcmp("vms", argv[3]) == 0)
        options.replacement_strategy= RSTRAT_VMS;
    else {
        fprintf(stderr, "Incorrect replacement strategy! [[File %s at line %d]]\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    options.quiet_mode= (strcmp("quiet", argv[4]) == 0);
    return options;
}