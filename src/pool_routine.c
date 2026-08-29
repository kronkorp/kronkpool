#include "kronkpool/macros/optimization.h"
#include "kronkpool/macros/types.h"
#include "pool.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/pthreadtypes.h>
#include "queue/queue.h"

void *kpThreadPool_routine(
    void *arg
)
{
    kpThreadPool *pool = (kpThreadPool *)arg;

    if (!pool) {
        return NULL;
    }
    while (kpTrue) {
        kpThreadTask *task;
        {
            pthread_mutex_lock(&pool->mutex);
            while (!pool->stop && queue_empty(&pool->queue)) {
                pthread_cond_wait(&pool->cond, &pool->mutex);
            }
            // FIXME: Queue empty really necessary ??
            if (pool->stop && queue_empty(&pool->queue)) {
                pthread_mutex_unlock(&pool->mutex);
                return NULL;
            }
            task = queue_front(&pool->queue);
            // TODO: Check if good before pop
            queue_pop(&pool->queue);
            pool->pendings--;
            pthread_mutex_unlock(&pool->mutex);
        }
        pool->runnings++;
        // NOTE: Should call task... with ctx
        task->handler(task->data);
        free(task);
        pool->runnings--;
    }
    return NULL;
}
