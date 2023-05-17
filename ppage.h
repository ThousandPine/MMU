#pragma once

/*
物理页块信息
*/
typedef struct ppage_block
{
    int page_num;        /* 包含页的数量 */
    int start_page_id;   /* 起始页编号 */
    struct ppage_block *next; /* 下一个页块 */
} ppage_block;

/*
物理页管理系统信息
*/
typedef struct
{
    int page_size;                  /* 单个页所占的空间，以字节为单位 */
    int page_num;                   /* 页的总数 */
    int free_page_num;              /* 空闲页数量 */
    char *mem;                           /* 内存空间 */
    struct ppage_block *free_block_list; /* 空闲页块链表 */
} ppage_system;

/*
===========
创建物理页表
===========
*/
ppage_system *ppage_system_create(int page_num, int page_size);

/*
===========
销毁物理页表
===========
*/
void ppage_system_destroy(ppage_system *ppage_sys);

/*
===================
申请指定数量的物理页
===================
可能包含多个物理页块，以链表的形式返回
空闲页数量不足时返回NULL

NOTE: 释放物理页时也需要使用该链表
*/
ppage_block *ppage_alloc(ppage_system *ppage_sys, int page_num);

/*
====================
释放申请到的物理页链表
====================
传入之前申请物理页得到的链表，将链表中的物理页标记为空闲
*/
void ppage_free(ppage_system *ppage_sys, ppage_block *block_list);

/*
===============
打印空闲页块链表
===============
*/
void ppage_print_free_block(ppage_system *ppage_sys);