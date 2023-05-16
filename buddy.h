#pragma once

/*
空闲内存段
*/
typedef struct free_segment
{
    unsigned start_addr; /* 起始地址 */
    unsigned size;       /* 段长度 */
    struct free_segment *next;
} free_segment;

/*
伙伴系统
*/
typedef struct
{
    unsigned max_order;           /* 最大幂次 */
    free_segment **free_seg_list; /* 链表头数组 */
} buddy_system;

/*
===========
创建伙伴系统
===========
根据最大幂次和初始内存大小，创建伙伴系统链表
幂次上限不得超过31
*/
buddy_system *buddy_system_create(unsigned max_order, unsigned size);

/*
===============
释放伙伴系统资源
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
申请空间的首地址保存在addr参数中
返回最后申请到的空间大小，申请失败时返回0

NOTE：申请到的空间大于等于size，并且皆为2的n次幂
*/
unsigned buddy_system_alloc(buddy_system *buddy_sys, unsigned *addr, unsigned size);

/*
====================
释放内存空间到伙伴系统
====================
释放的内存空间需要为2的n次幂，否则输出错误信息，超过伙伴系统
*/
void buddy_system_free(buddy_system *buddy_sys, unsigned start_addr, unsigned size);