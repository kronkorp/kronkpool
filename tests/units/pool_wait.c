#include <stdatomic.h>
#include <stddef.h>
#include <unistd.h>
#include "kronklab/kronklab.h"
#include "kronkpool/kronkpool.h"

static atomic_size_t g_done;

static void *__slow_task(void *p)
{
    (void)p;
    usleep(200);
    ++g_done;
    return NULL;
}

Test(pool_wait, wait_idle_runs_everything)
{
    kpThreadPool *p = kpThreadPool_create(4);

    g_done = 0;
    for (size_t i = 0; i < 500; ++i) {
        kpThreadPool_pushTask(p, &__slow_task, NULL);
    }
    kpThreadPool_waitIdle(p);
    AssertEq((size_t)g_done, 500, "all tasks done after waitIdle, got %zu", (size_t)g_done);
    // The pool is still usable
    for (size_t i = 0; i < 100; ++i) {
        kpThreadPool_pushTask(p, &__slow_task, NULL);
    }
    kpThreadPool_waitIdle(p);
    AssertEq((size_t)g_done, 600, "and reusable, got %zu", (size_t)g_done);
    kpThreadPool_destroy(p);
}

Test(pool_wait, wait_idle_on_idle_pool)
{
    kpThreadPool *p = kpThreadPool_create(2);

    kpThreadPool_waitIdle(p);
    kpThreadPool_waitIdle(NULL);
    kpThreadPool_destroy(p);
}

struct chain
{
    kpThreadPool *pool;
    struct chain *next;
};

static void *__chain_task(void *p)
{
    struct chain *c = (struct chain *)p;

    usleep(500);
    ++g_done;
    if (c->next) {
        kpThreadPool_pushTask(c->pool, &__chain_task, c->next);
    }
    return NULL;
}

Test(pool_wait, wait_idle_follows_new_tasks)
{
    kpThreadPool *p = kpThreadPool_create(3);
    struct chain c[6];

    for (size_t i = 0; i < 6; ++i) {
        c[i] = (struct chain){ p, i + 1 < 6 ? &c[i + 1] : NULL };
    }
    g_done = 0;
    kpThreadPool_pushTask(p, &__chain_task, &c[0]);
    kpThreadPool_waitIdle(p);
    AssertEq((size_t)g_done, 6, "tasks pushed by tasks are waited too, got %zu", (size_t)g_done);
    kpThreadPool_destroy(p);
}

// NOTE: The window between "task popped" and "task counted as running" used
// to make a pool look idle while a task was about to start.
Test(pool_wait, wait_idle_repeated)
{
    kpThreadPool *p = kpThreadPool_create(8);

    g_done = 0;
    for (size_t round = 1; round <= 300; ++round) {
        for (size_t i = 0; i < 8; ++i) {
            kpThreadPool_pushTask(p, &__slow_task, NULL);
        }
        kpThreadPool_waitIdle(p);
        AssertEq((size_t)g_done, round * 8, "round %zu: got %zu", round, (size_t)g_done);
    }
    kpThreadPool_destroy(p);
}
