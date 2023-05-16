#include "buddy.h"
#include <stdio.h>

int main()
{
    buddy_system buddy_sys;
    buddy_system_init(&buddy_sys, 31, 1000);
    buddy_system_print(&buddy_sys);

    buddy_system_push(&buddy_sys, 992, 5);
    buddy_system_print(&buddy_sys);

    buddy_system_close(&buddy_sys);
    puts("====\nDone\n====");
}