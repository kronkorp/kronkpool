#include "kronkpool/kronkpool.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bool is_prime(long n)
{
    if (n < 2) return false;
    for (long i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

static void *__compute(void *arg)
{
    long limit = (long)arg;
    long count = 0;
    for (long i = 2; i < limit; ++i) {
        if (is_prime(i)) {
            count++;
        }
    }
    return (void *)count;
}

int main(void)
{
    const size_t NUM_TASKS = 200;
    const long LIMIT = 100000;

    clock_t start_seq = clock();
    for (size_t i = 0; i < NUM_TASKS; ++i) {
        __compute((void *)LIMIT);
    }
    clock_t end_seq = clock();
    double seq_time = (double)(end_seq - start_seq) / CLOCKS_PER_SEC;
    printf("Single-threaded time: %.4f seconds\n", seq_time);

    kpThreadPool *p = kpThreadPool_create(10);
    struct timespec start_pool, end_pool;
    clock_gettime(CLOCK_MONOTONIC, &start_pool);

    for (size_t i = 0; i < NUM_TASKS; ++i) {
        kpThreadPool_pushTask(p, &__compute, (void *)LIMIT);
    }
    
    kpThreadPool_destroy(p);
    
    clock_gettime(CLOCK_MONOTONIC, &end_pool);
    double pool_time = (end_pool.tv_sec - start_pool.tv_sec) +
        (end_pool.tv_nsec - start_pool.tv_nsec) / 1e9;
    printf("Multi-threaded (10 workers) time: %.4f seconds\n", pool_time);

    return 0;
}