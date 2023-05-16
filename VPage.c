#include "VPage.h"
#include "PPage.h"
#include <stdio.h>
#include <stdlib.h>

/*
======================
建立虚拟页到物理页的映射
======================
*/
void VPage_set_map(VPage_table *VPage_t)
{
    PPage_block *p = VPage_t->PPage_list;
    for (unsigned i = 0, j = 0; i < VPage_t->page_num; i++, j++)
    {
        if (j >= p->page_num)
        {
            p = p->next;
            j = 0;
        }

        VPage_t->page_map[i] = p->start_page_id + j;
    }

    return;
}

/*
==================
申请并初始化虚拟页表
==================
根据申请的内存大小和物理页信息，创建对应数量的虚拟页，并申请和映射对应的数量的物理页空间
申请失败时返回NULL
*/
VPage_table *VPage_alloc(PPage_system *PPage_sys, int size)
{
    VPage_table *VPage_t = (VPage_table *)malloc(sizeof(VPage_t));

    /* 计算页数量 */
    VPage_t->page_num = size / PPage_sys->page_size;
    if (size % PPage_sys->page_size != 0) /* 向上取整 */
    {
        ++VPage_t->page_num;
    }

    /* 申请物理页 */
    VPage_t->PPage_list = PPage_alloc(PPage_sys, VPage_t->page_num);

    /* 申请失败时释放内存，并返回NULL */
    if (VPage_t->PPage_list == NULL)
    {
        free(VPage_t);
        return NULL;
    }

    /* 申请映射表空间 */
    VPage_t->page_map = (int *)malloc(sizeof(int) * VPage_t->page_num);

    /* 建立物理页映射 */
    VPage_set_map(VPage_t);

    return VPage_t;
}

/*
===============
释放虚拟页表资源
===============
*/
void VPage_free(PPage_system *PPage_sys, VPage_table *VPage_t)
{
    /* 释放物理页 */
    PPage_free(PPage_sys, VPage_t->PPage_list);

    /* 释放映射表 */
    free(VPage_t->page_map);

    /* 释放虚拟页表 */
    free(VPage_t);
}