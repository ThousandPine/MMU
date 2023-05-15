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

VPage_table *VPage_alloc(PPage_system *PPage_sys, int size);
void VPage_free(VPage_table *VPage_t, PPage_system *PPage_sys);
