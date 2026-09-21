#include "kronkpool/macros/optimization.h"
#include "kronkpool/kronkpool.h"
#include "pool.h"
#include <pthread.h>

KP_API
void kpThreadPool_waitIdle(
    kpThreadPool *pool
)
{
    if (!pool) {
        return;
    }
    pthread_mutex_lock(&pool->mutex);
    while (pool->pendings > 0 || pool->runnings > 0) {
        pthread_cond_wait(&pool->idle, &pool->mutex);
    }
    pthread_mutex_unlock(&pool->mutex);
}
