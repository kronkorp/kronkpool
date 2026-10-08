#include "kronkpool/macros/optimization.h"
#include "pool.h"
#include "kronkpool/kronkpool.h"
#include "queue/queue.h"
#include <stdlib.h>
#include <stdio.h>

static void kpThreadPool_clear(
    kpThreadPool * pool
)
{
    // NOTE: stop is set and broadcast with the mutex held. Without it, a worker
    // that already checked stop but is not in kpCond_wait yet misses the
    // broadcast and sleeps forever (kpThread_join below would never return).
    kpMutex_lock(&pool->mutex);
    pool->stop = true;
    kpCond_broadcast(&pool->cond);
    kpMutex_unlock(&pool->mutex);
    for (size_t i = 0; i < pool->workers; ++i) {
        kpThread_join(pool->threads[i]);
    }
    kpMutex_destroy(&pool->mutex);
    kpCond_destroy(&pool->cond);
    kpCond_destroy(&pool->idle);
    queue_clear(&pool->queue, NULL);
    free(pool->threads);
}

KP_API
void kpThreadPool_destroy(
    kpThreadPool * pool
)
{
    if (!pool)
        return;
    kpThreadPool_clear(pool);
    free(pool);
}
