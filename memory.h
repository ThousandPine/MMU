#pragma once

#include "segment.h"

/*
内存
*/
typedef struct
{
    int size;                      /* 物理内存大小 */
    int vsize;                     /* 虚拟内存大小 */
    int max_order;                 /* 伙伴系统最大幂次 */
    ppage_system *ppage_sys;       /* 物理页表 */
    segment_table *seg_table_list; /* 段表链表 */
} memory;

/*
========
创建内存
========
size:物理内存空间，page_size:物理页大小，vsize:虚拟内存空间，max_order:伙伴系统最大幂次
*/
memory *memory_create(int size, int page_size, int vsize, int max_order);

/*
========
销毁内存
========
*/
void memory_destroy(memory *mem);

/*
===========
注册进程记录
===========
在内存中创建一个进程记录，为其分配段表和进程id。
函数会返回申请到的进程id
*/
int memory_register_process(memory *mem);

/*
===========
注销进程记录
===========
*/
void memory_unregister_process(memory *mem, int pid);

/*
=======
申请内存
=======
返回内存首地址，失败时返回-1
*/
int memory_alloc(memory *mem, int pid, int size);

/*
=======
释放内存
=======
*/
void memory_free(memory *mem, int pid, int addr);
