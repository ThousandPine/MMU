#include "vpage.h"
#include "ppage.h"
#include <stdio.h>
#include <stdlib.h>

/*
======================
建立虚拟页到物理页的映射
======================
*/
void vpage_set_map(vpage_table *vpage_t)
{
    ppage_block *p = vpage_t->ppage_list;
    for (int i = 0, j = 0; i < vpage_t->page_num; i++, j++)
    {
        if (j >= p->page_num)
        {
            p = p->next;
            j = 0;
        }

        vpage_t->page_map[i] = p->start_page_id + j;
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
vpage_table *vpage_table_alloc(ppage_system *ppage_sys, int size)
{
    vpage_table *vpage_t = (vpage_table *)malloc(sizeof(vpage_t));

    /* 计算页数量 */
    vpage_t->page_num = size / ppage_sys->page_size;
    if (size % ppage_sys->page_size != 0) /* 向上取整 */
    {
        ++vpage_t->page_num;
    }

    /* 申请物理页 */
    vpage_t->ppage_list = ppage_alloc(ppage_sys, vpage_t->page_num);

    /* 申请失败时释放内存，并返回NULL */
    if (vpage_t->ppage_list == NULL)
    {
        free(vpage_t);
        return NULL;
    }

    /* 申请映射表空间 */
    vpage_t->page_map = (int *)malloc(sizeof(int) * vpage_t->page_num);

    /* 建立物理页映射 */
    vpage_set_map(vpage_t);

    return vpage_t;
}

/*
===============
释放虚拟页表资源
===============
*/
void vpage_table_free(ppage_system *ppage_sys, vpage_table *vpage_t)
{
    /* 释放物理页 */
    ppage_free(ppage_sys, vpage_t->ppage_list);

    /* 释放映射表 */
    free(vpage_t->page_map);

    /* 释放虚拟页表 */
    free(vpage_t);
}