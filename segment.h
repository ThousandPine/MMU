#pragma once

#include "buddy.h"
#include "vpage.h"
#include <pthread.h>

/*
内存段
*/
typedef struct segment
{
    unsigned start_addr;  /* 起始地址 */
    unsigned size;        /* 段长度 */
    vpage_table *vpage_t; /* 虚拟页表指针 */
    struct segment *next;
} segment;

/*
段表
*/
typedef struct segment_table
{
    pthread_t thread_id;     /* 所属线程 */
    segment *seg_list;       /* 段信息链表 */
    buddy_system *buddy_sys; /* 伙伴系统指针 */
    ppage_system *ppage_sys; /* 物理页表指针 */
    struct segment_table *next;
} segment_table;

/*
========
创建段表
========
*/
segment_table *segment_table_create(ppage_system *ppage_sys, pthread_t thread_id, unsigned size);

/*
===========
释放段表资源
===========
*/
void segment_table_destroy(segment_table *segment_t);

/*
=============
申请内存段空间
=============
申请空间并记录在段表，首地址保存在addr参数中，申请失败时函数返回-1
*/
int segment_alloc(segment_table *segment_t, unsigned *addr, unsigned size);

/*
=============
释放内存段空间
=============
传入申请时得到的内存段首地址，函数会在段表中查询并释放相应的段
*/
void segment_free(segment_table *segment_t, unsigned start_addr);
