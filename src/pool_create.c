#include "kronkpool/macros/optimization.h"
#include "kronkpool/macros/types.h"
#include "pool.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "queue/queue.h"

static int kfThreadPool_init(
    kpThreadPool *pool,
    size_t nthreads
)
{
    if (!pool) {
        return -1;
    }
    if (kpMutex_init(&pool->mutex) != 0) {
        return -1;
    }
    if (kpCond_init(&pool->cond) != 0) {
        return -1;
    }
    if (kpCond_init(&pool->idle) != 0) {
        return -1;
    }
    queue_init(&pool->queue);
    pool->pendings = 0;
    pool->runnings = 0;
    pool->workers = nthreads;
    pool->stop = kpFalse;
    pool->threads = calloc(nthreads, sizeof(kpThread));
    if (!pool->threads) {
        return -1;
    }
    for (size_t i = 0; i < pool->workers; ++i) {
        kpThread_create(&pool->threads[i], &kpThreadPool_routine, pool);
    }
    return 0;
}

KP_API
kpThreadPool *kpThreadPool_create(
    ssize_t concurrency
)
{
    kpThreadPool *pool = NULL;

    if (concurrency == 0) {
        return NULL;
    } else if (concurrency == -1) {
        concurrency = kpThread_cpuCount();
        if (concurrency <= 0) {
            return NULL;
        }
    }
    pool = calloc(1, sizeof(kpThreadPool));
    if (!pool) {
        return NULL;
    }
    // NOTE: Concurrency is already good, so init won't check...
    if (kfThreadPool_init(pool, concurrency) == -1) {
        free(pool);
        return NULL;
    }
    return pool;
}
