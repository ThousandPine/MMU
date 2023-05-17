#include "buddy.h"
#include <stdio.h>

int main()
{
    int addr;
    buddy_system *buddy_sys = buddy_system_create(30, (1 << 10));
    buddy_system_print(buddy_sys);

    addr = buddy_system_alloc(buddy_sys, 7);
    printf("addr=%d\n", addr);
    buddy_system_print(buddy_sys);

    buddy_system_free(buddy_sys, addr, 7);
    buddy_system_print(buddy_sys);

    buddy_system_destroy(buddy_sys);
    puts("====\nDone\n====");
}