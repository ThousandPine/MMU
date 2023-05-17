#include "buddy.h"
#include <stdio.h>
#include <stdlib.h>

/*
=============
创建空闲段信息
=============
*/
free_segment *free_segment_create(int start_addr, int size)
{
    free_segment *res = (free_segment *)malloc(sizeof(free_segment));
    res->start_addr = start_addr;
    res->size = size;
    res->next = NULL;
    return res;
}

/*
=============
销毁空闲段信息
=============
*/
void free_segment_destroy(free_segment *free_seg)
{
    if (free_seg != NULL)
        free(free_seg);
    return;
}

/* ============================================================================================= */

/*
=================
从伙伴系统申请内存
=================
从伙伴系统中申请大小为2的order次幂大小的内存，并返回首地址
申请失败时，返回-1
*/
int buddy_system_alloc(buddy_system *buddy_sys, int order)
{
    /* 1. 查找符合要求的最小内存块 */
    free_segment *free_seg = NULL;
    int f_order = order;

    while (f_order <= buddy_sys->max_order && free_seg == NULL)
    {
        free_seg = buddy_sys->free_seg_list[f_order];

        /* 找到内存块后，将其从伙伴系统移除 */
        if (free_seg != NULL)
        {
            buddy_sys->free_seg_list[f_order] = free_seg->next;
            // free_seg->next = NULL;
        }
        /* 否则继续寻找更大的块 */
        else
        {
            ++f_order;
        }
    }

    /* 未申请到可用空间则返回-1 */
    if (free_seg == NULL)
    {
        return -1;
    }

    /* 3. 当申请到的空间大于等于所需空间的两倍时，将后一半的空间放回伙伴系统 */
    for (int size = (1 << order); free_seg->size >= size * 2; --f_order, free_seg->size /= 2)
    {
        buddy_system_free(buddy_sys, free_seg->start_addr + (free_seg->size / 2), f_order - 1);
    }

    /* 5. 返回最后申请到的空间地址 */
    int addr = free_seg->start_addr;
    free_segment_destroy(free_seg);

    return addr;
}

/*
====================
释放内存空间到伙伴系统
====================
释放大小为2的order次幂的空间到伙伴系统
*/
void buddy_system_free(buddy_system *buddy_sys, int start_addr, int order)
{
    /* 1. 创建空闲内存段记录 */
    free_segment *free_seg = free_segment_create(start_addr, (1 << order));

    /* 2. 查找插入位置 */
    free_segment *p_pre_pre = NULL;
    free_segment *p_pre = NULL;
    free_segment *p = buddy_sys->free_seg_list[order];

    while (p != NULL && p->start_addr < start_addr)
    {
        p_pre_pre = p_pre;
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
    if (order < buddy_sys->max_order && p_pre != NULL && p_pre->start_addr + p_pre->size == free_seg->start_addr)
    {
        if (p_pre_pre == NULL) /* 涉及链表头 */
        {
            buddy_sys->free_seg_list[order] = free_seg->next;
        }
        else
        {
            p_pre_pre->next = free_seg->next;
        }

        buddy_system_free(buddy_sys, p_pre->start_addr, order + 1);
        free_segment_destroy(p_pre);
        free_segment_destroy(free_seg);
    }
    /* 与后一个合并 */
    else if (order < buddy_sys->max_order && p != NULL && free_seg->start_addr + free_seg->size == p->start_addr)
    {
        if (p_pre == NULL) /* 涉及链表头 */
        {
            buddy_sys->free_seg_list[order] = p->next;
        }
        else
        {
            p_pre->next = p->next;
        }

        buddy_system_free(buddy_sys, free_seg->start_addr, order + 1);
        free_segment_destroy(p);
        free_segment_destroy(free_seg);
    }

    return;
}

/* ============================================================================================= */

/*
===========
创建伙伴系统
===========
根据最大幂次和初始内存大小，创建伙伴系统链表
幂次上限不得超过30
*/
buddy_system *buddy_system_create(int max_order, int size)
{
    if (max_order > 30)
    {
        puts("ERROR buddy_system_init:: 最大幂次超过上限(30)");
        return NULL;
    }

    /* 分配空间 */
    buddy_system *buddy_sys = (buddy_system *)malloc(sizeof(buddy_system));
    buddy_sys->max_order = max_order;
    buddy_sys->free_seg_list = (free_segment **)malloc(sizeof(free_segment *) * (max_order + 1));

    /* 初始化链表数组 */
    for (int order = 0; order <= max_order; ++order)
    {
        buddy_sys->free_seg_list[order] = NULL;
    }

    /* 将内存分割并分配到对应大小的链表 */
    for (int start_addr = 0, order = max_order; order >= 0;)
    {
        int order_size = (1 << order);

        if (size < order_size)
        {
            --order;
        }
        else
        {
            buddy_system_free(buddy_sys, start_addr, order);
            start_addr += order_size;
            size -= order_size;
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
    /* 释放内存记录链表空间 */
    for (int order = 0; order <= buddy_sys->max_order; ++order)
    {
        free_segment *p_pre = NULL;
        free_segment *p = buddy_sys->free_seg_list[order];

        while (p != NULL)
        {
            p_pre = p;
            p = p->next;
            free_segment_destroy(p_pre);
        }
    }

    /* 释放链表头数组空间 */
    free(buddy_sys->free_seg_list);
}

/* ============================================================================================= */

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

        printf("order%u(%d)::", order, (1 << order));

        while (p != NULL)
        {
            printf("[%u-%u]", p->start_addr, p->start_addr + p->size - 1);
            if (p->next != NULL)
                printf(" -> ");
            p = p->next;
        }
        putchar('\n');
    }
    puts("=========================");
}
