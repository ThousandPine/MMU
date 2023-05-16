#include "buddy.h"
#include <stdio.h>

int main()
{
    unsigned addr;
    unsigned size;
    buddy_system *buddy_sys = buddy_system_create(31, (1 << 10));
    buddy_system_print(buddy_sys);

    size = buddy_system_alloc(buddy_sys, &addr, 1);
    printf("size=%u\n", size);
    buddy_system_print(buddy_sys);

    buddy_system_free(buddy_sys, addr, size);
    buddy_system_print(buddy_sys);

    buddy_system_destroy(buddy_sys);
    puts("====\nDone\n====");
}