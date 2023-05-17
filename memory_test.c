#include "memory.h"
#include <stdio.h>

int main()
{
    memory *mem = memory_create(1024, 4, 2048, 10);
    memory_printf_info(mem);

    const int num = 5;
    int pid[num];
    int addr[num];


    puts("register::");
    for (int i = 0; i < num; i++)
        pid[i] = memory_register_process(mem);
    memory_printf_info(mem);

    puts("alloc::");
    for (int i = 0; i < num; i++)
    {
        int size = (i + 1) * 257547 % 421;
        addr[i] = memory_alloc(mem, pid[i], size);
        printf("i=%d size=%d addr=%d\n", i, size, addr[i]);
    }
    memory_printf_info(mem);

    puts("free::");
    for (int i = 0; i < num; i++)
    {
        if(addr[i] != -1)
            memory_free(mem, pid[i], addr[i]);
    }
    memory_printf_info(mem);

    puts("unregister::");
    for (int i = 0; i < num; i++)
    {
        memory_unregister_process(mem, pid[i]);
    }
    memory_printf_info(mem);

    memory_destroy(mem);

    puts("====\nDone\n====");
    return 0;
}
