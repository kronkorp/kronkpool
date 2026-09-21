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
    // NOTE: stop is set and broadcast with the mutex held. Without it, a worker
    // that already checked stop but is not in pthread_cond_wait yet misses the
    // broadcast and sleeps forever (pthread_join below would never return).
    pthread_mutex_lock(&pool->mutex);
    pool->stop = true;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
    for (size_t i = 0; i < pool->workers; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    pthread_cond_destroy(&pool->idle);
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
