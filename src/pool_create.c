#include "kronkpool/macros/optimization.h"
#include "pool.h"
#include <stddef.h>
#include <sys/types.h>
#include <time.h>
#include <bits/pthreadtypes.h>
#include <stdlib.h>
#include <unistd.h>

static void *__routine(
    void *arg
)
{
    kpThreadPool *pool = (kpThreadPool *)arg;

    if (!pool) {
        return NULL;
    }
    while (true) {
        // kfThreadTask *task;
        // void *d;
        // pthread_mutex_lock(&pool->mutex);
        // while (!pool->stop && queue_empty(&pool->queue)) {
        //     pthread_cond_wait(&pool->cond, &pool->mutex);
        // }
        // // FIXME: Queue empty really necessary ??
        // if (pool->stop && queue_empty(&pool->queue)) {
        //     return NULL;
        // }
        // task = queue_front(&pool->queue);
        // queue_pop(&pool->queue);
        // pool->pendings--;
        // pthread_mutex_unlock(&pool->mutex);
        // pool->runnings++;
        // // NOTE: Should call task... with ctx
        // task->handler(task->data);
        // pool->runnings--;
    }
}

static int kfThreadPool_init(
    kpThreadPool *pool,
    size_t nthreads
)
{
    if (!pool) {
        return -1;
    }
    if (pthread_mutex_init(&pool->mutex, NULL) != 0) {
        return -1;
    }
    if (pthread_cond_init(&pool->cond, NULL) != 0) {
        return -1;
    }
    // queue_init(&pool->queue);
    pool->pendings = 0;
    pool->runnings = 0;
    // pool->ctx = ctx;
    pool->workers = nthreads;
    pool->stop = kpFalse;
    pool->threads = calloc(nthreads, sizeof(pthread_t));
    if (!pool->threads) {
        return -1;
    }
    for (size_t i = 0; i < nthreads; ++i) {
        pthread_create(&pool->threads[i], NULL, &__routine, pool);
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
        concurrency = sysconf(_SC_NPROCESSORS_ONLN);
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
