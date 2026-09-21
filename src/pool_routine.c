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
            // NOTE: Both changed under the same lock, so that a task is
            // always counted as pending or running (see kpThreadPool_waitIdle)
            pool->pendings--;
            pool->runnings++;
            pthread_mutex_unlock(&pool->mutex);
        }
        // NOTE: Should call task... with ctx
        task->handler(task->data);
        free(task);
        pthread_mutex_lock(&pool->mutex);
        pool->runnings--;
        if (pool->pendings == 0 && pool->runnings == 0) {
            pthread_cond_broadcast(&pool->idle);
        }
        pthread_mutex_unlock(&pool->mutex);
    }
    return NULL;
}
