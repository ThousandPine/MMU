#include <stdio.h>
#include "PPage.h"
#include "VPage.h"

int main()
{
    PPage_system PPage_sys;
    PPage_system_init(&PPage_sys, 1000, 4);
    PPage_print_free_block(&PPage_sys);

    VPage_table *VPage_t = VPage_alloc(&PPage_sys, 100);
    VPage_table *VPage_t1 = VPage_alloc(&PPage_sys, 200);
    PPage_print_free_block(&PPage_sys);

    VPage_free(&PPage_sys, VPage_t);
    PPage_print_free_block(&PPage_sys);

    VPage_free(&PPage_sys, VPage_t1);
    PPage_print_free_block(&PPage_sys);
}