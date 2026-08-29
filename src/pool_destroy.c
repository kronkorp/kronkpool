#include "kronkpool/macros/optimization.h"
#include "pool.h"
#include "kronkpool/kronkpool.h"
#include "queue/queue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static void kpThreadPool_clear(
    kpThreadPool * pool
)
{
    // NOTE: No need to lock / unlock since stop is now an atomic_bool
    pool->stop = true;
    pthread_cond_broadcast(&pool->cond);
    for (size_t i = 0; i < pool->workers; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
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
