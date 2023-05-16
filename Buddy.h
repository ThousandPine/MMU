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

void buddy_system_init(buddy_system *buddy_sys, unsigned max_order, unsigned size);
void buddy_system_close(buddy_system *buddy_sys);

void buddy_system_push(buddy_system *buddy_sys, unsigned start_addr, unsigned order);
unsigned buddy_system_pop(buddy_system *buddy_sys, unsigned order, int *flag);

void buddy_system_print(buddy_system *buddy_sys);