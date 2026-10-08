#include "kronkpool/macros/optimization.h"
#include "kronkpool/kronkpool.h"
#include "pool.h"

KP_API
void kpThreadPool_waitIdle(
    kpThreadPool *pool
)
{
    if (!pool) {
        return;
    }
    kpMutex_lock(&pool->mutex);
    while (pool->pendings > 0 || pool->runnings > 0) {
        kpCond_wait(&pool->idle, &pool->mutex);
    }
    kpMutex_unlock(&pool->mutex);
}
