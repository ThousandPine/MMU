#include "segment.h"
#include <stdio.h>

int main()
{
    ppage_system *ppage_sys = ppage_system_create(1000, 4);
    segment_table *segment_t = segment_table_create(ppage_sys, 1, 1024);
    buddy_system_print(segment_t->buddy_sys);
    ppage_print_free_block(segment_t->ppage_sys);
    puts("\n");

    int addr = 0;
    segment_alloc(segment_t, &addr, 1000);
    buddy_system_print(segment_t->buddy_sys);
    ppage_print_free_block(segment_t->ppage_sys);
    puts("\n");

    segment_free(segment_t, addr);
    buddy_system_print(segment_t->buddy_sys);
    ppage_print_free_block(segment_t->ppage_sys);
    puts("\n");

    segment_table_destroy(segment_t);
    ppage_system_destroy(ppage_sys);
    puts("====\nDone\n====");
    return 0;
}