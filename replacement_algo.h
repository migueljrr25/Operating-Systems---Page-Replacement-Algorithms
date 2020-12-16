#ifndef PGR_REPLACEMENT_ALGO_H
#define PGR_REPLACEMENT_ALGO_H

#include <stdio.h>
#include <stdbool.h>
#include "options.h"
#include "page.h"

// First In First Out
void fifo(t_page* page_table,
          t_hard_drive *hdd,
          unsigned int reference,
          char operation,
          t_options options,
          unsigned long time_elapsed) {

    if(!options.quiet_mode) {
        printf("\n======================================TRACE-LINE==============================================\n");
        printf("#%d\thex: %x\t\tpgn: %d\top: %c\n", time_elapsed, reference, reference/4096, operation);
        printf("==============================================================================================\n");
    }

    long index= find_page(page_table, options.nframes, reference, false);
    if(!options.quiet_mode)
        printf("Checking for a page hit...\n");

    if(index >= 0) {
        if(!options.quiet_mode)
            printf("...Page hit! Updating page stats...\n");

        update_page(page_table, index, reference, operation, time_elapsed);

        if(!options.quiet_mode) {
            printf("Printing page table content...\n");
            dump_page_table(page_table, options.nframes, false);
        }
        return;
    }

    // Page miss! We have to lookup on the hard drive, so a read must occur!
    ++hdd->total_disk_reads;

    if(!options.quiet_mode) {
        printf("Page miss! We have to lookup on the hard drive, so a read must occur!\n");
        printf("...Checking for available slot...\n");
    }

    index= has_available_slot(page_table, options.nframes);
    if(index >= 0) {

        if(!options.quiet_mode)
            printf("found empty page slot!\n");

        insert_new_page(page_table, index, reference, operation, time_elapsed);
        return;
    }

    if(!options.quiet_mode)
        printf("no more empty slots... starting a replacement...\n");

    index= select_page(page_table, options.nframes, PSC_INSERTION_TIME);
    if(!options.quiet_mode) {
        printf("oldest page at: %d(%x)\n", index, page_table[index].vaddress.page_number);
        printf("copying oldest page to disk...\n");
    }
    copy_page_to_disk(page_table, index, hdd, options); // this operation might increment the total_disk_writes! (iff page is dirty)
    insert_new_page(page_table, index, reference, operation, time_elapsed);

    if(!options.quiet_mode) {
        printf("Printing disk content...\n");
        dump_page_table(hdd->table, hdd->size, true);
        printf("Current disk reads: %d\n", hdd->total_disk_reads);
        printf("Current disk writes: %d\n", hdd->total_disk_writes);
    }

    if(!options.quiet_mode) {
        printf("Printing page table content...\n");
        dump_page_table(page_table, options.nframes, false);
    }
}

// Least Recently Used
void lru(t_page* page_table,
          t_hard_drive *hdd,
          unsigned int reference,
          char operation,
          t_options options,
          unsigned long time_elapsed) {

    if(!options.quiet_mode) {
        printf("\n======================================TRACE-LINE==============================================\n");
        printf("#%d\thex: %x\t\tpgn: %d\top: %c\n", time_elapsed, reference, reference/4096, operation);
        printf("==============================================================================================\n");
    }

    long index= find_page(page_table, options.nframes, reference, false);
    if(!options.quiet_mode)
        printf("Checking for a page hit...\n");

    if(index >= 0) {
        if(!options.quiet_mode)
            printf("...Page hit! Updating page stats...\n");

        update_page(page_table, index, reference, operation, time_elapsed);

        if(!options.quiet_mode) {
            printf("Printing page table content...\n");
            dump_page_table(page_table, options.nframes, false);
        }
        return;
    }

    // Page miss! We have to lookup on the hard drive, so a read must occur!
    ++hdd->total_disk_reads;

    if(!options.quiet_mode) {
        printf("Page miss! We have to lookup on the hard drive, so a read must occur!\n");
        printf("...Checking for available slot...\n");
    }

    index= has_available_slot(page_table, options.nframes);
    if(index >= 0) {

        if(!options.quiet_mode)
            printf("found empty page slot!\n");

        insert_new_page(page_table, index, reference, operation, time_elapsed);
        return;
    }

    if(!options.quiet_mode)
        printf("no more empty slots... starting a replacement...\n");

    index= select_page(page_table, options.nframes, PSC_LOWEST_USE_COUNT);
    if(!options.quiet_mode) {
        printf("oldest page at: %d(%x)\n", index, page_table[index].vaddress.page_number);
        printf("copying oldest page to disk...\n");
    }
    copy_page_to_disk(page_table, index, hdd, options); // this operation might increment the total_disk_writes! (iff page is dirty)
    insert_new_page(page_table, index, reference, operation, time_elapsed);

    if(!options.quiet_mode) {
        printf("Printing disk content...\n");
        dump_page_table(hdd->table, hdd->size, true);
        printf("Current disk reads: %d\n", hdd->total_disk_reads);
        printf("Current disk writes: %d\n", hdd->total_disk_writes);
    }

    if(!options.quiet_mode) {
        printf("Printing page table content...\n");
        dump_page_table(page_table, options.nframes, false);
    }
}

// Random
void rdm(t_page* page_table,
         t_hard_drive *hdd,
         unsigned int reference,
         char operation,
         t_options options,
         unsigned long time_elapsed) {

    if(!options.quiet_mode) {
        printf("\n======================================TRACE-LINE==============================================\n");
        printf("#%d\thex: %x\t\tpgn: %d\top: %c\n", time_elapsed, reference, reference/4096, operation);
        printf("==============================================================================================\n");
    }

    long index= find_page(page_table, options.nframes, reference, false);
    if(!options.quiet_mode)
        printf("Checking for a page hit...\n");

    if(index >= 0) {
        if(!options.quiet_mode)
            printf("...Page hit! Updating page stats...\n");

        update_page(page_table, index, reference, operation, time_elapsed);

        if(!options.quiet_mode) {
            printf("Printing page table content...\n");
            dump_page_table(page_table, options.nframes, false);
        }
        return;
    }

    // Page miss! We have to lookup on the hard drive, so a read must occur!
    ++hdd->total_disk_reads;

    if(!options.quiet_mode) {
        printf("Page miss! We have to lookup on the hard drive, so a read must occur!\n");
        printf("...Checking for available slot...\n");
    }

    index= has_available_slot(page_table, options.nframes);
    if(index >= 0) {

        if(!options.quiet_mode)
            printf("found empty page slot!\n");

        insert_new_page(page_table, index, reference, operation, time_elapsed);
        return;
    }

    if(!options.quiet_mode)
        printf("no more empty slots... starting a replacement...\n");

    index= select_page(page_table, options.nframes, PSC_RANDOM);
    if(!options.quiet_mode) {
        printf("oldest page at: %d(%x)\n", index, page_table[index].vaddress.page_number);
        printf("copying oldest page to disk...\n");
    }
    copy_page_to_disk(page_table, index, hdd, options); // this operation might increment the total_disk_writes! (iff page is dirty)
    insert_new_page(page_table, index, reference, operation, time_elapsed);

    if(!options.quiet_mode) {
        printf("Printing disk content...\n");
        dump_page_table(hdd->table, hdd->size, true);
        printf("Current disk reads: %d\n", hdd->total_disk_reads);
        printf("Current disk writes: %d\n", hdd->total_disk_writes);
    }

    if(!options.quiet_mode) {
        printf("Printing page table content...\n");
        dump_page_table(page_table, options.nframes, false);
    }
}

// VMS: Second Chance Algorithm
void vms(t_page* page_table,
         t_hard_drive *hdd,
         unsigned int process_number,
         t_page* page_table_fifo_process,
         t_page* page_table_fifo_clean,
         t_page* page_table_fifo_dirty,
         unsigned int reference,
         char operation,
         t_options options,
         unsigned long time_elapsed) {

    long index_page_clean_or_dirty;
    t_page* page_table_clean_or_dirty;
    unsigned int fifo_process_size= options.nframes/2;
    unsigned int page_table_clear_or_dirty_size= options.nframes/2 + 1;

    if(!options.quiet_mode) {
        printf("\n======================================TRACE-LINE==============================================\n");
        printf("#%d\thex: %x\t\tpgn: %d\top: %c\n", time_elapsed, reference, reference/4096, operation);
        printf("==============================================================================================\n");
    }

    long index= find_page(page_table_fifo_process, fifo_process_size, reference, false);
    if(!options.quiet_mode)
        printf("Checking for a page hit on FIFO-process#%d...\n", process_number);

    if(index >= 0) {
        if(!options.quiet_mode)
            printf("...Page hit! done...\n");

        update_page(page_table_fifo_process, index, reference, operation, time_elapsed);
        update_page(page_table, index, reference, operation, time_elapsed);
        return;
    }

    if(!options.quiet_mode) {
        printf("First level page miss! Inserting new page into FIFO-process#%d...\n", process_number);
        printf("...Checking for available slot on FIFO-process#%d...\n", process_number);
    }

    index= has_available_slot(page_table_fifo_process, fifo_process_size);
    if(index >= 0) {

        if(!options.quiet_mode)
            printf("found empty page slot on FIFO-process#%d!\n", process_number);

        insert_new_page(page_table_fifo_process, index, reference, operation, time_elapsed);
    } else {

        if(!options.quiet_mode)
            printf("no more empty slots... starting a replacement on FIFO-process#%d...\n", process_number);

        long index_page_out_fifo_process= select_page(page_table_fifo_process, fifo_process_size, PSC_INSERTION_TIME);
        if(!options.quiet_mode) {
            printf("oldest page at: %d(%x)\n", index_page_out_fifo_process, page_table_fifo_process[index_page_out_fifo_process].vaddress.page_number);
            printf("replacing oldest page...\n");
            printf("copying oldest page to clean/dirty list...\n");
        }

        if(!page_table_fifo_process[index_page_out_fifo_process].status.is_dirty)
            page_table_clean_or_dirty= page_table_fifo_clean;
        else
            page_table_clean_or_dirty= page_table_fifo_dirty;

        index_page_clean_or_dirty= has_available_slot(page_table_clean_or_dirty, page_table_clear_or_dirty_size);
        if(index_page_clean_or_dirty < 0)
            index_page_clean_or_dirty= select_page(page_table_clean_or_dirty, page_table_clear_or_dirty_size, PSC_INSERTION_TIME);

        page_table_clean_or_dirty[index_page_clean_or_dirty]= page_table_fifo_process[index_page_out_fifo_process];
        insert_new_page(page_table_fifo_process, index_page_out_fifo_process, reference, operation, time_elapsed);
    }

    index= find_page(page_table, options.nframes, reference, false);
    if(index >= 0) {
        // if new page is already in memory, try to remove it from clean or dirty list!
        // lookup new page on clean list
        index_page_clean_or_dirty= find_page(page_table_fifo_clean, page_table_clear_or_dirty_size, reference, false);
        page_table_clean_or_dirty= page_table_fifo_clean;
        if(index_page_clean_or_dirty < 0) {
            // lookup new page on dirty list
            index_page_clean_or_dirty= find_page(page_table_fifo_dirty, page_table_clear_or_dirty_size, reference, false);
            page_table_clean_or_dirty= page_table_fifo_dirty;
        }

        // if found new page then remove it from dirty/clean
        if(index_page_clean_or_dirty >= 0)
            init_page_slot(page_table_clean_or_dirty, index_page_clean_or_dirty);
    } else {

        // Page miss on memory, gotta load page from disk!
        ++hdd->total_disk_reads;

        index= has_available_slot(page_table, options.nframes);
        if(index >= 0)
            insert_new_page(page_table, index, reference, operation, time_elapsed);
        else {

            index_page_clean_or_dirty= select_page(page_table_fifo_clean, page_table_clear_or_dirty_size, PSC_INSERTION_TIME);
            page_table_clean_or_dirty= page_table_fifo_clean;

            if(index_page_clean_or_dirty < 0) {

                index_page_clean_or_dirty= select_page(page_table_fifo_dirty, page_table_clear_or_dirty_size, PSC_INSERTION_TIME);
                page_table_clean_or_dirty= page_table_fifo_dirty;
            }

            //page_table_clean_or_dirty[index_page_clean_or_dirty].vaddress.reference
            index= find_page(page_table,
                             options.nframes,
                             page_table_clean_or_dirty[index_page_clean_or_dirty].vaddress.reference,
                             false);

            if(page_table_clean_or_dirty[index_page_clean_or_dirty].status.is_dirty)
                ++hdd->total_disk_writes; // page to be replaced is dirty, save it to disk!

            // remove first clean/dirty
            init_page_slot(page_table_clean_or_dirty, index_page_clean_or_dirty);
            insert_new_page(page_table, index, reference, operation, time_elapsed);
        }
    }
}
#endif //PGR_REPLACEMENT_ALGO_H
