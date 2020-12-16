#include <stdlib.h>
#include <stdio.h>
#include "page.h"
#include "options.h"

void init_page_slot(t_page* table, long index) {
    // statistics
    table[index].page_stats.read_count= 0;
    table[index].page_stats.write_count= 0;
    table[index].page_stats.use_count= 0;
    // status
    table[index].status.is_dirty= false;
    table[index].status.is_available= true;
    table[index].status.last_modified_time= 0;
    table[index].status.insertion_time= 0;
    // virtual address
    table[index].vaddress.offset= 0;
    table[index].vaddress.page_number= 0;
    table[index].vaddress.reference= 0;
}

void init_table(t_page* table, unsigned long size) {
    for(unsigned long i=0; i < size; ++i)
        init_page_slot(table, i);
}

void init_hard_drive(t_hard_drive *hdd) {
    hdd->size= __HDD_SIZE__;
    hdd->total_disk_writes=0;
    hdd->total_disk_reads=0;
    hdd->last_item= 0;

    for(unsigned long i=0; i < hdd->size; ++i)
        init_page_slot(hdd->table, i);
}

void insert_new_page(t_page* table, long index, unsigned int reference, char operation, unsigned long time) {

    init_page_slot(table, index);
    table[index].status.insertion_time= time;
    update_page(table, index, reference, operation, time);
}

void insert_page(t_page* table, long index, t_page page, unsigned int reference, char operation, unsigned long time) {

    table[index]= page;
    table[index].status.insertion_time= time;
    update_page(table, index, reference, operation, time);
}

void update_page(t_page* table, long index, unsigned int reference, char operation, unsigned long time) {

    table[index].status.is_available= false; // make sure the "slot" / "page entry" is occupied with a page
    if(operation == 'r' || operation == 'R')
        ++table[index].page_stats.read_count;

    if(operation == 'w' || operation == 'W') {
        table[index].status.is_dirty= true;
        ++table[index].page_stats.write_count;
    }

    ++table[index].page_stats.use_count;
    table[index].status.last_modified_time= time;
    table[index].vaddress.reference= reference;
    table[index].vaddress.page_number= reference / 4096;
    table[index].vaddress.offset= reference % 4096;
}

void copy_page_to_disk(t_page* table, long index, t_hard_drive *hdd, t_options options) {

    table[index].page_stats.use_count= 0;
    if(table[index].status.is_dirty) {
        //if page was dity that counts as a write to disk!
        ++hdd->total_disk_writes;
        table[index].status.is_dirty= false;
        if(!options.quiet_mode)
            printf("\n...page was dirty that counts as a write to disk!\n");
    }

    //Simulate save to disk...
    long index_hdd= find_page(hdd->table, hdd->size, table[index].vaddress.reference, true);
    if(index_hdd < 0) {
        // if page not found on disk then push back page
        if(hdd->last_item < hdd->size) {
            index_hdd= hdd->last_item;
            ++hdd->last_item;
        }
    }

    if(index_hdd >= 0) {
        // save page to disk
        hdd->table[index_hdd]= table[index];
    }
}

long find_page(t_page* table, unsigned long size, unsigned int reference, bool stop_on_empty_page) {

    unsigned int pgn= reference / 4096;
    for(long i=0; i < size; ++i) {
        if(stop_on_empty_page && table[i].status.is_available)
            return -1;

        if(!table[i].status.is_available && // if page is not empty...
            pgn == table[i].vaddress.page_number)
            return i;
    }
    return -1; // didn't find the page
}

long has_available_slot(t_page* table, unsigned long size) {

    for(long i=0; i < size; ++i)
        if(table[i].status.is_available)
            return i;
    return -1; // didn't find any available slot in the page table
}

long select_page(t_page* table, unsigned long size, t_page_selection_criteria criteria) {

    if(size == 0)
        return -1;

    if(criteria == PSC_RANDOM)
        return (rand() % size);

    long selection= -1;
    for(long i=0; i < size; ++i) {

        if(!table[i].status.is_available && selection < 0) {
            selection= i;
            continue;
        }

        switch(criteria) {
            case PSC_INSERTION_TIME: // handy for FIFO
                if(!table[i].status.is_available &&
                   table[selection].status.insertion_time > table[i].status.insertion_time)
                    selection= i; // the oldest page is the one with smallest timestamp...
            break;

            case PSC_LOWEST_USE_COUNT: // LRU
                if(!table[i].status.is_available &&
                   table[selection].page_stats.use_count  > table[i].page_stats.use_count)
                    selection= i;
            break;
        }
    }

    return selection;
}

void dump_page_table(t_page* table, unsigned long size, bool stop_on_empty_page) {

    printf("\n#\t|PGN\t|D\t|RC\t|WC\t|UC\n");
    for(long i=0; i < size; ++i) {

        if(table[i].status.is_available) {
            if(stop_on_empty_page) {
                if(i==0)
                    printf("\t---EMPTY---\n");
                return;
            }
            printf("%d\t|---\t|-\t|--\t|--\t\n", i);
            continue;
        }
        printf("%d\t|%x\t|%d\t|%d\t|%d\t|%d\t\n",
                i,
                table[i].vaddress.page_number,
                table[i].status.is_dirty,
                table[i].page_stats.read_count,
                table[i].page_stats.write_count,
               table[i].page_stats.use_count);
    }
}


