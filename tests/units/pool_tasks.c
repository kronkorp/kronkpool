#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "kronklab/kronklab.h"
#include "kronkpool/kronkpool.h"

struct payload
{
    void *ret;
    void *arg;
};

static void *__dummy_task(
    void *p
)
{
    struct payload *payload = (struct payload*)p;

    payload->ret = (void *)((long int)payload->arg * 2);
    return NULL;
}

Test(pool_tasks, basic_task)
{
    kpThreadPool *p = kpThreadPool_create(4);

    struct payload pl = {0, (void *)10};

    kpThreadPool_pushTask(p, &__dummy_task, &pl);
    kpThreadPool_destroy(p);    
    AssertEq((long int)pl.ret, 10 * 2, "Payload ret should have been %d, got %ld", 10 * 2, (long int)pl.ret);
}

struct atomic_payload
{
    atomic_size_t visitors;
};

static void *__visite(void *p)
{
    struct atomic_payload *pl = (struct atomic_payload *)p;

    ++pl->visitors;
    return NULL;
}

Test(pool_tasks, lot_of_tasks)
{
    kpThreadPool *p = kpThreadPool_create(10);

    struct atomic_payload pl = {0};

    size_t f = 100;
    for (size_t i = 0; i < f; ++i) {
        kpThreadPool_pushTask(p, &__visite, &pl);
    }
    kpThreadPool_destroy(p);    
    AssertEq((size_t)pl.visitors, f, "Payload ret should have been %zu, got %zu", f, (size_t)pl.visitors);
}