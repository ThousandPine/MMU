/*
物理页块信息
*/
typedef struct PPage_block
{
    unsigned page_num;        /* 包含页的数量 */
    unsigned start_page_id;   /* 起始页编号 */
    struct PPage_block *next; /* 下一个页块 */
} PPage_block;

/*
物理页管理系统信息
*/
typedef struct
{
    unsigned page_size;                 /* 单个页所占的空间，以字节为单位 */
    unsigned page_num;                  /* 页的总数 */
    unsigned free_page_num;             /* 空闲页数量 */
    char *mem;                          /* 内存空间 */
    struct PPage_block *free_page_list; /* 空闲页块链表 */
} PPage_system;

PPage_block *PPage_block_create(unsigned start_page_id, unsigned page_num);
void PPage_system_init(PPage_system *PPage_sys, unsigned page_num, unsigned page_size);
PPage_block *PPage_alloc(PPage_system *PPage_sys, unsigned page_num);
void PPage_free(PPage_system *PPage_sys, PPage_block *block);
void PPage_print_free_block(PPage_system *PPage_sys);