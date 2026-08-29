#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "kronklab/kronklab.h"
#include "kronkpool/kronkpool.h"

Test(pool_creation, basic_creation)
{
    kpThreadPool *p = kpThreadPool_create(4);

    AssertNe(p, NULL, "Threadpool should have been created");
    kpThreadPool_destroy(p);
}

Test(pool_creation, invalid_creation)
{
    kpThreadPool *p = kpThreadPool_create(0);

    AssertEq(p, NULL, "Threadpool should not have been created");
    kpThreadPool_destroy(p);
}

Test(pool_creation, check_workers)
{
    kpThreadPool *p = kpThreadPool_create(10);

    size_t workers = kpThreadPool_getWorkers(p);
    AssertEq(workers, 10, "Threadpool should have 10 workers, got %zu", workers);
    kpThreadPool_destroy(p);
}

Test(pool_creation, check_workers_max)
{
    kpThreadPool *p = kpThreadPool_create(-1);

    size_t conf = sysconf(_SC_NPROCESSORS_ONLN);
    size_t workers = kpThreadPool_getWorkers(p);
    AssertEq(workers, conf, "Threadpool should have %zu workers, got %zu", conf, workers);
    kpThreadPool_destroy(p);
}

Test(pool_creation, check_workers_bigbig)
{
    kpThreadPool *p = kpThreadPool_create(128);

    size_t workers = kpThreadPool_getWorkers(p);
    AssertEq(workers, 128, "Threadpool should have 128 workers, got %zu", workers);
    kpThreadPool_destroy(p);
}
