typedef struct PPage_block
{
    int page_num;
    int start_page_id;
    struct PPage_block *next;
} PPage_block;

typedef struct
{
    int page_size;
    int page_num;
    int free_page_num;
    char *mem;
    struct PPage_block *free_page_list;
} PPage_system;


