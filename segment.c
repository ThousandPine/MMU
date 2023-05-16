#include "segment.h"
#include <stdio.h>
#include <stdlib.h>

/*
=============
申请内存段空间
=============
申请空间并记录在段表，首地址保存在addr参数中，申请失败时函数返回-1
*/
int segment_alloc(segment_table *segment_t, unsigned *addr, unsigned size)
{
    segment *seg = (segment *)malloc(sizeof(segment));

    /* 1. 从伙伴系统申请内存 */
    seg->size = buddy_system_alloc(segment_t->buddy_sys, &seg->start_addr, size);

    /* 申请失败则返回NULL */
    if (seg->size == 0)
    {
        free(seg);
        return -1;
    }

    /* 2. 创建虚拟页 */
    seg->vpage_t = vpage_table_alloc(segment_t->ppage_sys, size);

    /* 创建失败则返回NULL */
    if (seg->vpage_t == NULL)
    {
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

    /* 4. 记录结果并返回 */
    *addr = seg->start_addr;

    return 0;
}

/*
=============
释放内存段空间
=============
传入申请时得到的内存段首地址，函数会在段表中查询并释放相应的段
*/
void segment_free(segment_table *segment_t, unsigned start_addr)
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
    buddy_system_free(segment_t->buddy_sys, p->start_addr, p->size);

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
segment_table *segment_table_create(ppage_system *ppage_sys, unsigned pid, unsigned size)
{
    segment_table *segment_t = (segment_table *)malloc(sizeof(segment_table));
    segment_t->seg_list = NULL;
    segment_t->pid = pid;
    segment_t->ppage_sys = ppage_sys;
    segment_t->buddy_sys = buddy_system_create(31, size);
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
