// 请注意这只是一个单独演示，要加入wmkc_thread库中请根据需要进行增删。
#include <wmkc_conf.h>
#include <wmkc_memory.c>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

// 线程句柄对象
typedef struct {
    pthread_t th; // 线程句柄
    sem_t sem;    // 信号量
} thread_handle_obj;

// 线程对象
typedef struct {
    thread_handle_obj *handle; // 线程句柄对象
    void *args;                // 要传入函数的参数的指针
} thread_obj;

void *func(void *args)
{
    thread_obj *thread = (thread_obj *)args;
    wU32 number = *((wU32 *)thread->args);
    sem_wait(&thread->handle->sem);
    printf("the function is running.\n");
    printf("the function parameter: %u\n", number);
}

int main(int argc, char **argv)
{
    thread_obj *obj = wmkcNull;
    wU32 number = 166;

    wmkcMem_new(thread_obj *, obj, sizeof(thread_obj));
    wmkcMem_new(thread_handle_obj *, obj->handle, sizeof(thread_handle_obj));
    obj->args = &number;

    // 初始化信号量
    sem_init(&obj->handle->sem, 0, 0);
    pthread_create(&obj->handle->th, NULL, func, obj);

    // 唤醒被挂起的函数
    sem_post(&obj->handle->sem);
    pthread_join(obj->handle->th, NULL);

    // 销毁信号量
    sem_destroy(&obj->handle->sem);

    wmkcMem_free(obj->handle);
    wmkcMem_free(obj);
    return 0;
}

