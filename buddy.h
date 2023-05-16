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
=============
初始化伙伴系统
=============
根据最大幂次和初始内存大小，创建伙伴系统链表
幂次上限不得超过31
*/
void buddy_system_init(buddy_system *buddy_sys, unsigned max_order, unsigned size);

/*
===============
释放伙伴系统资源
===============
*/
void buddy_system_close(buddy_system *buddy_sys);

/*
==================
添加内存段到伙伴系统
==================
添加大小为2的order次幂的空间到伙伴系统
*/
void buddy_system_push(buddy_system *buddy_sys, unsigned start_addr, unsigned order);

/*
==================
从伙伴系统移除内存块
==================
移除大小为2的order次幂的内存记录，并返回起始地址。
操作成功时flag置为1，失败时flag置为0
*/
unsigned buddy_system_pop(buddy_system *buddy_sys, unsigned order, int *flag);

/*
===================
打印伙伴系统链表信息
===================
*/
void buddy_system_print(buddy_system *buddy_sys);