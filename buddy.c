#include "buddy.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
=============
创建空闲段信息
=============
*/
free_segment *free_segment_create(unsigned start_addr, unsigned size)
{
    free_segment *res = (free_segment *)malloc(sizeof(free_segment));
    res->start_addr = start_addr;
    res->size = size;
    res->next = NULL;
    return res;
}

/*
=============
释放空闲段信息
=============
*/
void free_segment_destroy(free_segment *free_seg)
{
    if (free_seg != NULL)
        free(free_seg);
    return;
}

/*
=================
释放空闲段链表信息
=================
*/
void free_segment_list_destroy(free_segment *free_seg)
{
    free_segment *p_pre = NULL;
    free_segment *p = free_seg;

    while (p != NULL)
    {
        p_pre = p;
        p = p->next;
        free_segment_destroy(p_pre);
    }
    return;
}

/* ============================================================================================= */

/*
===========
创建伙伴系统
===========
根据最大幂次和初始内存大小，创建伙伴系统链表
幂次上限不得超过31
*/
buddy_system *buddy_system_create(unsigned max_order, unsigned size)
{
    if (max_order > 31)
    {
        puts("ERROR buddy_system_init:: 最大幂次超过上限(31)");
        return NULL;
    }
    buddy_system *buddy_sys = (buddy_system *)malloc(sizeof(buddy_system));
    buddy_sys->max_order = max_order;
    buddy_sys->free_seg_list = (free_segment **)malloc(sizeof(free_segment *) * (max_order + 1));

    /* 将内存分割到对应大小的链表 */
    unsigned start_addr = 0;

    for (int order = max_order; order >= 0; --order)
    {
        unsigned layer_size = (1 << order);

        if ((layer_size & size) == 0)
        {
            buddy_sys->free_seg_list[order] = NULL;
        }
        else
        {
            buddy_sys->free_seg_list[order] = free_segment_create(start_addr, layer_size);
            start_addr += layer_size;
        }
    }

    return buddy_sys;
}

/*
===============
释放伙伴系统资源
===============
*/
void buddy_system_destroy(buddy_system *buddy_sys)
{
    for (int order = 0; order <= buddy_sys->max_order; ++order)
    {
        free_segment_list_destroy(buddy_sys->free_seg_list[order]);
    }

    free(buddy_sys->free_seg_list);
}

/*
==================
添加内存段到伙伴系统
==================
添加大小为2的order次幂的空间到伙伴系统
*/
void buddy_system_push(buddy_system *buddy_sys, unsigned start_addr, unsigned order)
{
    /* 1. 创建空闲内存段 */
    free_segment *free_seg = free_segment_create(start_addr, (1 << order));

    /* 2. 查找插入位置 */
    free_segment *p_pre = NULL;
    free_segment *p = buddy_sys->free_seg_list[order];

    while (p != NULL && p->start_addr < start_addr)
    {
        p_pre = p;
        p = p->next;
    }

    /* 3. 插入链表 */
    /* 插入点在表头 */
    if (p_pre == NULL)
    {
        free_seg->next = buddy_sys->free_seg_list[order];
        buddy_sys->free_seg_list[order] = free_seg;
    }
    /* 插入点在表尾或中间 */
    else
    {
        free_seg->next = p;
        p_pre->next = free_seg;
    }

    /* 4. 相邻合并(递归) */
    /* 与前一个合并 */
    if (p_pre != NULL && p_pre->start_addr + p_pre->size == free_seg->start_addr)
    {
        buddy_sys->free_seg_list[order] = free_seg->next;

        buddy_system_push(buddy_sys, p_pre->start_addr, order + 1);
        free_segment_destroy(p_pre);
        free_segment_destroy(free_seg);
    }
    /* 与后一个合并 */
    else if (p != NULL && free_seg->start_addr + free_seg->size == p->start_addr)
    {
        if (p_pre == NULL) /* 新加入段在表头 */
        {
            buddy_sys->free_seg_list[order] = p->next;
        }
        else
        {
            p_pre->next = p->next;
        }

        buddy_system_push(buddy_sys, free_seg->start_addr, order + 1);
        free_segment_destroy(p);
        free_segment_destroy(free_seg);
    }

    return;
}

/*
==================
从伙伴系统移除内存块
==================
移除大小为2的order次幂的内存记录，并返回空闲段记录指针，无对应记录则返回NULL。
*/
free_segment *buddy_system_pop(buddy_system *buddy_sys, unsigned order)
{
    free_segment *p = buddy_sys->free_seg_list[order];
    if (p != NULL)
    {
        buddy_sys->free_seg_list[order] = p->next;
        p->next = NULL;
    }

    return p;
}

/*
===================
打印伙伴系统链表信息
===================
*/
void buddy_system_print(buddy_system *buddy_sys)
{
    puts("=======buddy system======");
    for (int order = buddy_sys->max_order; order >= 0; --order)
    {
        free_segment *p = buddy_sys->free_seg_list[order];

        if (p == NULL)
        {
            continue;
        }

        printf("order%u::", order);

        while (p != NULL)
        {
            printf("[start:%u end:%u]", p->start_addr, p->start_addr + p->size);
            if (p->next != NULL)
                printf(" -> ");
            p = p->next;
        }
        putchar('\n');
    }
    puts("=========================");
}

/*
=================
从伙伴系统申请内存
=================
申请空间的首地址保存在addr参数中
返回最后申请到的空间大小，申请失败时返回0

NOTE：申请到的空间大于等于size，并且皆为2的n次幂
*/
unsigned buddy_system_alloc(buddy_system *buddy_sys, unsigned *addr, unsigned size)
{
    /* 1. 计算最小所需的幂次 */
    unsigned order = 0;

    for (unsigned x = 1; x < size; x <<= 1)
        ++order;

    /* 2. 从伙伴系统申请可用空间 */
    free_segment *free_seg = NULL;

    while (order <= buddy_sys->max_order)
    {
        free_seg = buddy_system_pop(buddy_sys, order);
        if (free_seg != NULL)
            break;
        ++order;
    }

    /* 未申请到可用空间则返回-1，表示失败 */
    if (free_seg == NULL)
    {
        return 0;
    }

    /* 3. 记录首地址 */
    *addr = free_seg->start_addr;

    /* 4. 当申请到的空间大于等于所需空间的两倍时，将后一半的空间放回伙伴系统 */
    while (free_seg->size >= size * 2)
    {
        --order;
        free_seg->size /= 2;
        buddy_system_push(buddy_sys, free_seg->start_addr + free_seg->size, order);
    }

    /* 5. 返回最后申请到的空间大小 */
    size = free_seg->size;
    free_segment_destroy(free_seg);

    return size;
}

/*
====================
释放内存空间到伙伴系统
====================
释放的内存空间需要为2的n次幂，否则输出错误信息，超过伙伴系统
*/
void buddy_system_free(buddy_system *buddy_sys, unsigned start_addr, unsigned size)
{
    /* 使用BrianKernighan算法判断是否为2的n次幂 */
    if ((size & (size - 1)) != 0)
    {
        puts("ERROR buddy_system_free:: 释放的内存空间需要为2的n次幂");
        return;
    }

    /* 计算对应的幂次 */
    unsigned order = 0;

    while(order <= buddy_sys->max_order && size != (1 << order))
    {
        ++order;
    }

    /* 添加记录到伙伴系统 */
    buddy_system_push(buddy_sys, start_addr, order);

    return;
}