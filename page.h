#ifndef PGR_PAGE_H
#define PGR_PAGE_H

#include <stdbool.h>
#include "options.h"

    typedef struct t_page_virtual_address {
        unsigned int page_number;
        unsigned int offset;
        unsigned int reference;
    } t_page_virtual_address;

    typedef struct t_page_statistics {
        unsigned int read_count;
        unsigned int write_count;
        unsigned int use_count; // handy for LRU
    } t_page_statistics;

    typedef struct t_page_state {
        bool is_dirty;
        bool is_available;
        unsigned long last_modified_time;
        unsigned long insertion_time;
    } t_page_state;

    typedef struct t_page {
        t_page_virtual_address vaddress;
        t_page_statistics page_stats;
        t_page_state status;
    } t_page;

    #define __HDD_SIZE__ 20000

    typedef struct t_hard_drive {
        unsigned long last_item;
        unsigned long size;
        t_page table[__HDD_SIZE__];
        unsigned long total_disk_reads;
        unsigned long total_disk_writes;
    } t_hard_drive;

    typedef enum t_page_selection_criteria {
        PSC_INSERTION_TIME,
        PSC_LOWEST_USE_COUNT,
        PSC_RANDOM
    } t_page_selection_criteria;

    void init_page_slot(t_page* table, long index);
    void init_table(t_page* table, unsigned long size);
    void init_hard_drive(t_hard_drive *hdd);
    void insert_new_page(t_page* table, long index, unsigned int reference, char operation, unsigned long time);
    void insert_page(t_page* table, long index, t_page page, unsigned int reference, char operation, unsigned long time);
    void update_page(t_page* table, long index, unsigned int reference, char operation, unsigned long time);
    long has_available_slot(t_page* table, unsigned long size);
    long select_page(t_page* table, unsigned long size, t_page_selection_criteria criteria);
    void copy_page_to_disk(t_page* table, long index, t_hard_drive *hdd, t_options options);
    long find_page(t_page* table, unsigned long size, unsigned int reference, bool stop_on_empty_page);

    void dump_page_table(t_page* table, unsigned long size, bool stop_on_empty_page);

#endif //PGR_PAGE_H
