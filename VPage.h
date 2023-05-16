#include "PPage.h"

/*
虚拟页表
*/
typedef struct
{
    unsigned page_num;       /* 虚拟页数量 */
    int *page_map;           /* 页号映射表 */
    PPage_block *PPage_list; /* 申请的物理页块信息 */
} VPage_table;

/*
==================
申请并初始化虚拟页表
==================
根据申请的内存大小和物理页信息，创建对应数量的虚拟页，并申请和映射对应的数量的物理页空间
申请失败时返回NULL
*/
VPage_table *VPage_alloc(PPage_system *PPage_sys, int size);

/*
===============
释放虚拟页表资源
===============
*/
void VPage_free(PPage_system *PPage_sys, VPage_table *VPage_t);
