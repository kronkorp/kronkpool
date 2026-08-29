#include "kronkpool/kronkpool.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void *__print(void *a)
{
    for (int i = 0; i < (long int)a; ++i) {
        printf("hello from %zu\n", pthread_self());
    }
    return NULL;
}

__attribute__((constructor))
void init_rand(void)
{
    srand(time(NULL));
}

int main(void)
{
    srand(time(NULL));
    kpThreadPool *p = kpThreadPool_create(10);

    for (size_t i = 0; i < 1000; ++i) {
        // kpThreadPool_pushTask(p, &__print, (void *)10000);
        __print((void *)(10000));
    }
    kpThreadPool_destroy(p);
    return 0;
}
