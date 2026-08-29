#include <pthread.h>
#include <kronkpool/kronkpool.h>
#include <stdio.h>
#include <unistd.h>

// Your task handler
static void *my_task(void *arg)
{
    printf("Task executed in thread [%zu] with arg [%p]\n", pthread_self(), arg);
    return NULL;
}

int main(void)
{
    // Create a threadpool with [HARDWARE_CONCURRENCY] workers
    kpThreadPool *p = kpThreadPool_create(-1);

    // Get the number of workers
    printf("Threadpool has %zu workers\n", kpThreadPool_getWorkers(p));

    // Push a task
    kpThreadPool_pushTask(p, &my_task, (void *)10);

    // Destroy the pool
    kpThreadPool_destroy(p);
}
