#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <unistd.h>
// #include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include "memory.h"

#define PHYSICAL_MEM_SIZE 4096 /* 物理内存空间 */
#define PAGE_SIZE 4            /* 单个页面大小 */
#define VIRTUAL_MEM_SIZE 4096  /* 虚拟内存空间 */
#define BUDDY_MAX_ORDER 11     /* 伙伴系统最大幂次 */

#define PROCESS_NUM 4 /* 进程数 */

memory *mem;
sem_t mutex;

void sleep(int n)
{

    for (int i = 0, n = 100 * rand(); i < n; ++i)
        ;
}

/*
模拟进程行为
*/
void *process(void *arg)
{
    /* 初始化 */
    sem_wait(&mutex);

    int pid = memory_register_process(mem); /* 注册内存空间 */

    printf("process_init::pid=%d\n\n", pid); /* 输出初始化信息 */
    memory_printf_info(mem);                 /* 输出内存信息 */

    sem_post(&mutex);

    sleep(1);

    /* 申请内存 */
    const int cnt = 2 + (rand() % 3); /* 随机设定申请次数 */
    int addr[cnt];                    /* 内存地址 */
    int size[cnt];                    /* 申请空间 */

    for (int i = 0; i < cnt; ++i)
    {
        sem_wait(&mutex);

        size[i] = 1 + (rand() % 2048);             /* 随机设定申请的内存空间 */
        addr[i] = memory_alloc(mem, pid, size[i]); /* 申请内存 */

        printf("alloc::pid=%d size=%d addr=%d\n\n", pid, size[i], addr[i]); /* 输出申请信息和地址 */
        memory_printf_info_by_pid(mem, pid);                                /* 输出内存信息 */

        sem_post(&mutex);
        sleep(1);
    }

    /* 释放内存 */
    for (int i = 0; i < cnt; ++i)
    {
        sem_wait(&mutex);

        if (addr[i] != -1)
        {
            memory_free(mem, pid, addr[i]); /* 释放内存 */

            printf("free::pid=%d size=%d\n\n", pid, size[i]); /* 输出释放信息 */
            memory_printf_info_by_pid(mem, pid);              /* 输出内存信息 */
        }

        sem_post(&mutex);
        sleep(1);
    }

    /* 释放资源并结束 */
    sem_wait(&mutex);
    memory_unregister_process(mem, pid);
    // printf("process_finish::pid=%d\n\n", pid); /* 输出进程结束信息 */
    // memory_printf_info(mem);                   /* 输出内存信息 */
    sem_post(&mutex);
    return NULL;
}

int main()
{
    /* 初始化 */
    srand(time(NULL));                                                                    /* 设置随机种子 */
    sem_init(&mutex, 0, 1);                                                               /* 初始化信号量 */
    mem = memory_create(PHYSICAL_MEM_SIZE, PAGE_SIZE, VIRTUAL_MEM_SIZE, BUDDY_MAX_ORDER); /* 创建内存 */

    memory_printf_info(mem); /* 输出内存初始状态 */

    /* 创建线程，模拟多进程运行 */
    pthread_t thread[PROCESS_NUM];

    for (int i = 0; i < PROCESS_NUM; ++i)
    {
        int res = pthread_create(&thread[i], NULL, process, NULL);

        if (res != 0)
        {
            printf("ERROR:: 线程创建失败(code %d)\n", res);
            return -1;
        }
    }

    /* 等待线程结束 */
    for (int i = 0; i < PROCESS_NUM; ++i)
    {
        pthread_join(thread[i], NULL);
    }

    memory_printf_info(mem); /* 输出内存最后状态 */

    /* 释放资源 */
    memory_destroy(mem);
    sem_destroy(&mutex);
    puts("====\nDone\n====");

    return 0;
}