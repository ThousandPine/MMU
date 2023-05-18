#pragma once

/*
空闲内存段
*/
typedef struct free_segment
{
    int start_addr; /* 起始地址 */
    int size;       /* 段长度 */
    struct free_segment *next;
} free_segment;

/*
伙伴系统
*/
typedef struct
{
    int max_order;                /* 最大幂次 */
    free_segment **free_seg_list; /* 链表头数组 */
} buddy_system;

/*
===========
创建伙伴系统
===========
根据最大幂次和初始内存大小，创建伙伴系统链表
幂次上限不得超过30
*/
buddy_system *buddy_system_create(int max_order, int size);

/*
===============
销毁伙伴系统资源
===============
*/
void buddy_system_destroy(buddy_system *buddy_sys);

/*
===================
打印伙伴系统链表信息
===================
*/
void buddy_system_print(buddy_system *buddy_sys);

/*
=================
从伙伴系统申请内存
=================
从伙伴系统中申请大小为2的order次幂大小的内存，并返回首地址
申请失败时，返回-1
*/
int buddy_system_alloc(buddy_system *buddy_sys, int order);

/*
====================
释放内存空间到伙伴系统
====================
释放大小为2的order次幂的空间到伙伴系统
*/
void buddy_system_free(buddy_system *buddy_sys, int start_addr, int order);
