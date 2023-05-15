#include <stdio.h>
#include "PPage.h"

int main()
{
    PPage_system PPage_sys;
    PPage_system_init(&PPage_sys, 1000, 4);
    while (1)
    {
        unsigned x;
        printf("Operation 1.alloc 2.free:");
        scanf("%u", &x);
        /* alloc */
        if (x == 1)
        {
            printf("Input alloc page num:");
            scanf("%u", &x);
            PPage_alloc(&PPage_sys, x);
        }
        else if(x == 2)
        {
            PPage_block block;
            printf("Input start page id:");
            scanf("%u", &block.start_page_id);
            printf("Input page num:");
            scanf("%u", &block.page_num);
            PPage_free(&PPage_sys, &block);
        }
        else
            break;

        PPage_print_free_block(&PPage_sys);
        puts("\n");
    }
    PPage_system_close(&PPage_sys);
    puts("====\nExit\n====");
    return 0;
}