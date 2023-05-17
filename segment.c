#include "segment.h"
#include <stdio.h>
#include <stdlib.h>

/*
=============
申请内存段空间
=============
申请空间并记录在段表，返回内存的首地址，申请失败时返回-1
*/
int segment_alloc(segment_table *segment_t, int size)
{
    segment *seg = (segment *)malloc(sizeof(segment));

    /* 1. 计算size对应的2的最小幂次 */

    /* Brian Kernighan算法判断size是否为2的幂，若不是则需要让结果额外加1 */
    seg->order = (size & (size - 1)) == 0 ? -1 : 0;

    while (size > 0)
    {
        ++(seg->order);
        size >>= 1;
    }

    /* 1. 从伙伴系统申请内存 */
    seg->start_addr = buddy_system_alloc(segment_t->buddy_sys, seg->order);
    seg->size = (1 << seg->order);

    /* 申请失败则返回-1 */
    if (seg->start_addr == -1)
    {
        free(seg);
        return -1;
    }

    /* 2. 分配虚拟页表 */
    seg->vpage_t = vpage_table_alloc(segment_t->ppage_sys, seg->size);

    /* 创建失败则返回-1 */
    if (seg->vpage_t == NULL)
    {
        buddy_system_free(segment_t->buddy_sys, seg->start_addr, seg->order);
        free(seg);
        return -1;
    }

    /* 3. 将申请到的段加入段表记录 */
    segment *p_pre = NULL;
    segment *p = segment_t->seg_list;

    while (p != NULL && p->start_addr < seg->start_addr)
    {
        p_pre = p;
        p = p->next;
    }

    if (p_pre == NULL)
    {
        seg->next = p;
        segment_t->seg_list = seg;
    }
    else
    {
        p_pre->next = seg;
        seg->next = p;
    }

    /* 4. 返回结果 */

    return seg->start_addr;
}

/*
=============
释放内存段空间
=============
传入申请时得到的内存段首地址，函数会在段表中查询并释放相应的段
*/
void segment_free(segment_table *segment_t, int start_addr)
{
    /* 1. 找到对应地址的段记录 */
    segment *p_pre = NULL;
    segment *p = segment_t->seg_list;

    while (p != NULL && p->start_addr != start_addr)
    {
        p_pre = p;
        p = p->next;
    }

    /* 未找到时输出错误信息，并提前退出 */
    if (p == NULL)
    {
        puts("ERROR segment_free:: 释放失败，未找到相应地址的段记录");
        return;
    }

    /* 2. 释放内存到伙伴系统 */
    buddy_system_free(segment_t->buddy_sys, p->start_addr, p->order);

    /* 3. 释放虚拟页表 */
    vpage_table_free(segment_t->ppage_sys, p->vpage_t);

    /* 4. 从段记录链表移出 */
    if (p_pre == NULL)
    {
        segment_t->seg_list = p->next;
    }
    else
    {
        p_pre->next = p->next;
    }
    free(p);
}

/* ========================================================================= */

/*
========
创建段表
========
*/
segment_table *segment_table_create(unsigned pid, ppage_system *ppage_sys, int size, unsigned max_order)
{
    segment_table *segment_t = (segment_table *)malloc(sizeof(segment_table));
    segment_t->seg_list = NULL;
    segment_t->pid = pid;
    segment_t->ppage_sys = ppage_sys;
    segment_t->buddy_sys = buddy_system_create(max_order, size);
    segment_t->next = NULL;

    return segment_t;
}

/*
===========
释放段表资源
===========
*/
void segment_table_destroy(segment_table *segment_t)
{
    /* 释放申请的段空间 */
    segment *p_pre = NULL;
    segment *p = segment_t->seg_list;

    while (p != NULL)
    {
        p_pre = p;
        p = p->next;
        segment_free(segment_t, p->start_addr);
    }

    /* 释放伙伴系统空间 */
    buddy_system_destroy(segment_t->buddy_sys);

    /* 释放段表空间 */
    free(segment_t);
}
