#include "PPage.h"
#include <stdlib.h>

/*
===============
创建物理页块记录
===============
*/
PPage_block *PPage_block_create(unsigned start_page_id, unsigned page_num)
{
    PPage_block *block = (PPage_block *)malloc(sizeof(PPage_block));
    block->page_num = page_num;
    block->start_page_id = start_page_id;
    block->next = NULL;
    return block;
}

/*
=============
初始化物理页表
=============
*/
void PPage_system_init(PPage_system *PPage_sys, unsigned page_num, unsigned page_size)
{
    PPage_sys->page_size = page_size; /* 每个页的大小，以字节为单位 */
    PPage_sys->page_num = PPage_sys->free_page_num = page_num;
    PPage_sys->mem = (char *)malloc(sizeof(char) * page_num * page_size);
    PPage_sys->free_page_list = PPage_block_create(0, page_num);
    return;
}

/*
===================
申请指定数量的物理页
===================
可能包含多个物理页块，以链表的形式返回
空闲页数量不足时返回NULL
*/
PPage_block *PPage_alloc(PPage_system *PPage_sys, unsigned page_num)
{
    /* 空闲物理页数量不足，返回NULL */
    if (PPage_sys->free_page_num < page_num)
        return NULL;

    /* 1. 找到满足页数量要求的位置 */
    PPage_block *p = PPage_sys->free_page_list;
    int cnt = p->page_num;
    
    while(cnt < page_num && p->next != NULL) /* 统计页数量 */
    {
        p = p->next;
        cnt += p->page_num;
    }

    /* 2. 分割链表末尾页块中多余的页 */
    if(cnt > page_num)
    {
        int delta = cnt - page_num; /* 计算多余页数量 */

        p->page_num -= delta;   /* 减去多余的页 */

        PPage_block *new_block = PPage_block_create(p->start_page_id + p->page_num - delta, delta); /* 将多余的页放入新的页块 */

        /* 将新的页块插入空闲页链表 */
        new_block->next = p->next;
        p->next = new_block;
    }

    /* 3. 修改空闲页和申请页链表，并返回结果 */
    PPage_block *res = PPage_sys->free_page_list;
    PPage_sys->free_page_list = p->next;
    p->next = NULL;

    return res;
}

/* 
=============
释放页块中的页
=============
只会释放当前传入的页块，并不会释放整个链表
*/
void PPage_free(PPage_system *PPage_sys, PPage_block *block)
{
    /* 1. 增加空闲页数量并创建页块副本 */
    PPage_sys->free_page_num += block->page_num;
    block = PPage_block_create(block->start_page_id, block->page_num);  /* 创建副本，用于加入队列 */

    /* 2. 找到插入位置 */
    PPage_block *p_pre;     /* 前指针 */
    PPage_block *p = PPage_sys->free_page_list;    /* 后指针 */

    while(p != NULL && p->start_page_id < block->start_page_id) /* 找到第一个起始页id比它大的页块 */
    {
        p_pre = p;
        p = p->next;
    }

    /* 3. 插入链表，要分两种情况 */

    /* 3.1 插入位置在链表头 */
    if (p_pre == NULL)
    {
        PPage_sys->free_page_list = block;
    }
    /* 3.2 插入位置在链表中间或链表尾 */
    else
    {
        block->next = p;
        p_pre->next = block;
    }

    /* 4. 判断能否与前后页块合并 */
    /* 合并前页块 */
    if (p_pre != NULL && p_pre->start_page_id + p_pre->page_num == block->start_page_id)
    {
        p_pre->page_num += block->page_num;
        p_pre->next = block->next;
        free(block);
        block = p_pre;
    }
    /* 合并后页块 */
    if(p != NULL && block->start_page_id + block->page_num == p->start_page_id)
    {
        block->page_num += p->page_num;
        block->next = p->next;
        free(p);
    }

    return;
}