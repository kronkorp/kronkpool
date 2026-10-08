/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Smoke test, with no test framework: runs on every platform.
*/
#include "kronkpool/kronkpool.h"
#include <stdio.h>

#define TASKS 1000

static kpThreadPool *g_pool = NULL;
static int g_done[TASKS];
static int g_later[TASKS];

static void *later(void *arg)
{
    g_later[(int *)arg - g_done] = 1;
    return NULL;
}

// NOTE: Each task writes its own slot: nothing is shared, nothing to lock.
//       Half of them push one more task, which waitIdle must wait for too
static void *task(void *arg)
{
    int *slot = arg;

    *slot = 1;
    if ((slot - g_done) % 2 == 0) {
        kpThreadPool_pushTask(g_pool, &later, slot);
    }
    return NULL;
}

static int check(int ok, const char *what)
{
    printf("%s: %s\n", ok ? "ok" : "FAILED", what);
    return ok ? 0 : 1;
}

int main(void)
{
    int failed = 0;
    int done = 0;
    int later_done = 0;

    g_pool = kpThreadPool_create(-1);
    failed += check(g_pool != NULL, "a pool with a worker per processor");
    if (!g_pool) {
        return 1;
    }
    failed += check(kpThreadPool_getWorkers(g_pool) > 0, "it has workers");
    for (int i = 0; i < TASKS; ++i) {
        failed += kpThreadPool_pushTask(g_pool, &task, &g_done[i]) != 0;
    }
    kpThreadPool_waitIdle(g_pool);
    for (int i = 0; i < TASKS; ++i) {
        done += g_done[i];
        later_done += g_later[i];
    }
    failed += check(done == TASKS, "every task ran");
    failed += check(later_done == TASKS / 2, "and every task they pushed, before waitIdle returned");
    kpThreadPool_destroy(g_pool);

    g_pool = kpThreadPool_create(3);
    failed += check(g_pool != NULL && kpThreadPool_getWorkers(g_pool) == 3, "a pool of 3 workers");
    kpThreadPool_destroy(g_pool);
    failed += check(kpThreadPool_create(0) == NULL, "no pool of 0 workers");

    printf("%s\n", failed ? "smoke test FAILED" : "smoke test passed");
    return failed ? 1 : 0;
}
