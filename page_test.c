#include <stdio.h>
#include "ppage.h"
#include "vpage.h"

int main()
{
    ppage_system *ppage_sys = ppage_system_create(1000, 4);
    ppage_print_free_block(ppage_sys);

    vpage_table *vpage_t = vpage_table_alloc(ppage_sys, 100);
    vpage_table *vpage_t1 = vpage_table_alloc(ppage_sys, 200);
    ppage_print_free_block(ppage_sys);

    vpage_table_free(ppage_sys, vpage_t);
    ppage_print_free_block(ppage_sys);

    vpage_table_free(ppage_sys, vpage_t1);
    ppage_print_free_block(ppage_sys);

    ppage_system_destroy(ppage_sys);
    puts("====\nDone\n====");
}