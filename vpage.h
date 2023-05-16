#include "ppage.h"

/*
虚拟页表
*/
typedef struct
{
    unsigned page_num;       /* 虚拟页数量 */
    int *page_map;           /* 页号映射表 */
    ppage_block *ppage_list; /* 申请的物理页块信息 */
} vpage_table;

/*
==================
申请并初始化虚拟页表
==================
根据申请的内存大小和物理页信息，创建对应数量的虚拟页，并申请和映射对应的数量的物理页空间
申请失败时返回NULL
*/
vpage_table *vpage_alloc(ppage_system *ppage_sys, int size);

/*
===============
释放虚拟页表资源
===============
*/
void vpage_free(ppage_system *ppage_sys, vpage_table *vpage_t);
