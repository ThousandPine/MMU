#include "buddy.h"
#include <stdio.h>

int main()
{
    unsigned addr;
    buddy_system *buddy_sys = buddy_system_create(31, 1000);
    buddy_system_print(buddy_sys);

    buddy_system_free(buddy_sys, 1000, 24);
    buddy_system_print(buddy_sys);

    buddy_system_alloc(buddy_sys, &addr, 1000);
    buddy_system_print(buddy_sys);

    buddy_system_destroy(buddy_sys);
    puts("====\nDone\n====");
}