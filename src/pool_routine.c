#include "kronkpool/macros/optimization.h"
#include "kronkpool/macros/types.h"
#include "pool.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
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
            kpMutex_lock(&pool->mutex);
            while (!pool->stop && queue_empty(&pool->queue)) {
                kpCond_wait(&pool->cond, &pool->mutex);
            }
            // FIXME: Queue empty really necessary ??
            if (pool->stop && queue_empty(&pool->queue)) {
                kpMutex_unlock(&pool->mutex);
                return NULL;
            }
            task = queue_front(&pool->queue);
            // TODO: Check if good before pop
            queue_pop(&pool->queue);
            // NOTE: Both changed under the same lock, so that a task is
            // always counted as pending or running (see kpThreadPool_waitIdle)
            pool->pendings--;
            pool->runnings++;
            kpMutex_unlock(&pool->mutex);
        }
        // NOTE: Should call task... with ctx
        task->handler(task->data);
        free(task);
        kpMutex_lock(&pool->mutex);
        pool->runnings--;
        if (pool->pendings == 0 && pool->runnings == 0) {
            kpCond_broadcast(&pool->idle);
        }
        kpMutex_unlock(&pool->mutex);
    }
    return NULL;
}
