#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "options.h"
#include "page.h"
#include "replacement_algo.h"

/*
    process 1: All references that have a leading hex-digit of "3",
               E.g. 0x003AF342, 0x3F5643... (leading zeroes are ignored)
    process 2: otherwise
*/
unsigned int get_process(unsigned int ref) {
    unsigned int mask=          0xF0000000;
    unsigned int value_to_find= 0x30000000;

    while(mask != 0) {
        unsigned int res= (ref & mask);

        //printf("m: %x vf: %x res: %x ref: %x\n", mask, value_to_find, res, ref);
        if(res == value_to_find)
            return 1;// this is process 1: starts with hex value of "3"

        if(res != 0)
            break; // we stop here, the leading hex-digit is not "3"!

        // leading zeroes are ignored so move forward
        mask = mask >> 4;
        value_to_find= value_to_find >> 4;
    }

    return 2;// this is process 2: ref doesn't start with hex value of "3"
}

int main(int argc, char* argv[]) {

    if (argc < 5) {
        fprintf(stderr, "Not enough arguments in the execution. Usage: 'memsim <tracefile> <nframes> <rdm|lru|fifo|vms> <debug|quiet>");
        exit(EXIT_FAILURE);
    }

    t_options options= get_options(argc, argv);
    FILE *fp = fopen(options.file,"r");
    if (fp == NULL)
    {
        fprintf(stderr,"Failed to open tracefile. [[File %s at line %d]]\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    unsigned long time_elapsed= 0;
    unsigned int reference;
    char operation; // Read / Write
    // page tables
    t_page page_table[options.nframes];
    t_page* page_table_fifo_process;// for vms only
    unsigned int ps;// for vms only
    t_page page_table_fifo_p1[options.nframes/2];// for vms only
    t_page page_table_fifo_p2[options.nframes/2];// for vms only
    t_page page_table_fifo_clean[options.nframes/2 + 1];// for vms only
    t_page page_table_fifo_dirty[options.nframes/2 + 1];// for vms only
    // hard drive
    t_hard_drive hdd;

    init_hard_drive(&hdd);
    init_table(page_table, options.nframes);
    // initialize aux vms page tables
    init_table(page_table_fifo_p1, options.nframes/2);
    init_table(page_table_fifo_p2, options.nframes/2);
    init_table(page_table_fifo_clean, options.nframes/2 + 1);
    init_table(page_table_fifo_dirty, options.nframes/2 + 1);

    while (fscanf(fp, "%x %c", &reference, &operation) != EOF) {
        ++time_elapsed;

        switch(options.replacement_strategy) {
            case RSTRAT_FIFO:
                fifo(page_table, &hdd, reference, operation, options, time_elapsed);
            break;

            case RSTRAT_LRU:
                lru(page_table, &hdd, reference, operation, options, time_elapsed);
            break;

            case RSTRAT_RDM:
                rdm(page_table, &hdd, reference, operation, options, time_elapsed);
            break;

            case RSTRAT_VMS:

                ps = get_process(reference);
                if(ps == 1)
                    page_table_fifo_process= page_table_fifo_p1;
                else
                    page_table_fifo_process= page_table_fifo_p2;

                vms(page_table, &hdd, ps, page_table_fifo_process, page_table_fifo_clean, page_table_fifo_dirty, reference, operation, options, time_elapsed);

                if(!options.quiet_mode) {
                    printf("\n=============================\n");
                    printf("\n=============FIFO-P1================\n");
                    dump_page_table(page_table_fifo_p1, options.nframes/2, true);
                    printf("\n=============FIFO-P2================\n");
                    dump_page_table(page_table_fifo_p2, options.nframes/2, true);
                    printf("\n=============FIFO-CLEAN================\n");
                    dump_page_table(page_table_fifo_clean, options.nframes/2 + 1, true);
                    printf("\n=============FIFO-DIRTY================\n");
                    dump_page_table(page_table_fifo_dirty, options.nframes/2 + 1, true);
                    printf("\n=============FIFO-MEM================\n");
                    dump_page_table(page_table, options.nframes, true);// Memory
                }
            break;
        }
    }

    if(!options.quiet_mode) {
        printf("-------------------------------END---------------------------------------------------\n");
        printf("\nPrinting disk content...\n");
        dump_page_table(hdd.table, hdd.size, true);

        printf("\nPrinting page table content...\n");
        dump_page_table(page_table, options.nframes, true);
    }

    printf("\n\ntotal memory frames: %d\n", options.nframes);
    printf("events in trace: %d\n", time_elapsed);
    printf("total disk reads: %d\n", hdd.total_disk_reads);
    printf("total disk writes: %d\n", hdd.total_disk_writes);
    fclose(fp);
    return 0;
}
