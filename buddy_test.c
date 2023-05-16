#include "buddy.h"
#include <stdio.h>

int main()
{
    int res;
    buddy_system buddy_sys;
    buddy_system_init(&buddy_sys, 31, 1000);
    buddy_system_print(&buddy_sys);

    buddy_system_free(&buddy_sys, 1000, 24);
    buddy_system_print(&buddy_sys);

    buddy_system_alloc(&buddy_sys, 24, &res);
    buddy_system_print(&buddy_sys);

    buddy_system_close(&buddy_sys);
    puts("====\nDone\n====");
}