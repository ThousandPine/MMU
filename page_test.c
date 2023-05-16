#include <stdio.h>
#include "ppage.h"
#include "vpage.h"

int main()
{
    ppage_system ppage_sys;
    ppage_system_init(&ppage_sys, 1000, 4);
    ppage_print_free_block(&ppage_sys);

    vpage_table *vpage_t = vpage_alloc(&ppage_sys, 100);
    vpage_table *vpage_t1 = vpage_alloc(&ppage_sys, 200);
    ppage_print_free_block(&ppage_sys);

    vpage_free(&ppage_sys, vpage_t);
    ppage_print_free_block(&ppage_sys);

    vpage_free(&ppage_sys, vpage_t1);
    ppage_print_free_block(&ppage_sys);

    ppage_system_close(&ppage_sys);
    puts("====\nDone\n====");
}