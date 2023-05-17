#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

/*
========
创建内存
========
size:物理内存空间，page_size:物理页大小，vsize:虚拟内存空间，max_order:伙伴系统最大幂次
*/
memory *memory_create(int size, int page_size, int vsize, int max_order)
{
    if (size % page_size != 0)
    {
        puts("ERROR memory_create:: 内存空间必须为页大小的整数倍");
        return NULL;
    }

    memory *mem = (memory *)malloc(sizeof(memory));
    mem->size = size;
    mem->ppage_sys = ppage_system_create(size / page_size, page_size);
    mem->vsize = vsize;
    mem->max_order = max_order;
    mem->seg_table_list = NULL;

    return mem;
}

/*
========
销毁内存
========
*/
void memory_destroy(memory *mem)
{
    segment_table *p_pre = NULL;
    segment_table *p = mem->seg_table_list;
    while (p != NULL)
    {
        p_pre = p;
        p = p->next;
        segment_table_destroy(p_pre);
    }

    ppage_system_destroy(mem->ppage_sys);

    free(mem);
}

/* ================================================================= */

/*
===========
注册进程记录
===========
在内存中创建一个进程记录，为其分配段表和进程id。
函数会返回申请到的进程id
*/
int memory_register_process(memory *mem)
{
    /* 按顺序查找空闲的进程id和插入位置 */
    int pid = 0;
    segment_table *p_pre = NULL;
    segment_table *p = mem->seg_table_list;

    while (p != NULL && p->pid == pid)
    {
        ++pid;
        p_pre = p;
        p = p->next;
    }

    /* 创建内存段记录并插入链表 */
    segment_table *segment_t = segment_table_create(pid, mem->ppage_sys, mem->vsize, mem->max_order);

    if (p_pre == NULL)
    {
        mem->seg_table_list = segment_t;
        segment_t->next = p;
    }
    else
    {
        p_pre->next = segment_t;
        segment_t->next = p;
    }

    /* 返回进程id */
    return pid;
}

/*
===========
注销进程记录
===========
*/
void memory_unregister_process(memory *mem, int pid)
{
    /* 查找进程 */
    segment_table *p_pre = NULL;
    segment_table *p = mem->seg_table_list;

    while (p != NULL && p->pid != pid)
    {
        p_pre = p;
        p = p->next;
    }

    /* 未找到对应进程，输出错误信息 */
    if(p == NULL)
    {
        puts("ERROR memory_unregister_process:: 进程id不存在");
        return;
    }

    /* 将进程从链表中移除 */
    if (p_pre == NULL)
    {
        mem->seg_table_list = p->next;
    }
    else
    {
        p_pre->next = p->next;
    }

    /* 销毁段表记录 */
    segment_table_destroy(p);
    
    return;
}

/* ================================================================= */

/*
=======
申请内存
=======
返回内存首地址，失败时返回-1
*/
int memory_alloc(memory *mem, int pid, int size)
{
    /* 查找进程段表 */
    segment_table *segment_t = mem->seg_table_list;

    while(segment_t!=NULL && segment_t->pid != pid)
    {
        segment_t = segment_t->next;
    }

    /* 未找到则输出错误信息并返回-1 */
    if(segment_t == NULL)
    {
        puts("ERROR memory_alloc:: 进程id未注册");
        return -1;
    }

    /* 向段表申请内存并返回地址 */
    int addr = segment_alloc(segment_t, size);

    return addr;
}

/*
=======
释放内存
=======
*/
void memory_free(memory *mem, int pid, int addr)
{
    /* 查找进程段表 */
    segment_table *segment_t = mem->seg_table_list;

    while(segment_t!=NULL && segment_t->pid != pid)
    {
        segment_t = segment_t->next;
    }

    /* 未找到则输出错误信息 */
    if(segment_t == NULL)
    {
        puts("ERROR memory_free:: 进程id未注册");
        return;
    }

    /* 释放内存到段表 */
    segment_free(segment_t, addr);

    return;
}

/* ================================================================= */

/*
===========
输出内存信息
===========
*/
void memory_printf_info(memory *mem)
{
    puts("***********Memory info***********\n");

    /* 物理内存 */
    ppage_print_free_block(mem->ppage_sys);
    putchar('\n');
    // puts("\n");

    /* 虚拟内存 */
    segment_table *p = mem->seg_table_list;

    while(p != NULL)
    {
        printf("pid: %d\n", p->pid);

        buddy_system_print(p->buddy_sys);
        putchar('\n');

        p = p->next;
    }

    // puts("*********************************\n");
    return;
}