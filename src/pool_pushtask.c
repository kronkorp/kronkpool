#include "kronkpool/macros/optimization.h"
#include "pool.h"
#include "kronkpool/kronkpool.h"
#include "queue/queue.h"
#include <stdlib.h>

KP_API
int kpThreadPool_pushTask(
    kpThreadPool *pool,
    void *(*task)(void *),
    void *data
)
{
    kpThreadTask *p = NULL;

    if (!pool) {
        return -1;
    }
    p = calloc(1, sizeof(kpThreadTask));
    if (!p) {
        return -1;
    }
    p->data = data;
    p->handler = task;
    pthread_mutex_lock(&pool->mutex);
    queue_push(&pool->queue, p);
    ++pool->pendings;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}
