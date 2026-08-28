#include "kronkpool/macros/optimization.h"
#include "pool.h"
#include "kronkpool/kronkpool.h"
#include <stdlib.h>

static void kpThreadPool_clear(
    kpThreadPool * pool
)
{
    // pthread_mutex_lock(&pool->mutex);
    pool->stop = true;
    // pthread_mutex_unlock(&pool->mutex);
    for (size_t i = 0; i < pool->workers; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    // queue_destroy(&pool->queue, NULL);
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
