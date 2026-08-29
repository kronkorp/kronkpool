#include "kronkpool/kronkpool.h"
#include <stdio.h>

static void *__print(void *a)
{
    for (int i = 0; i < (long int)a; ++i) {
        printf("%ld\n", (long int)a);
    }
    return NULL;
}

int main(void)
{
    kpThreadPool *p = kpThreadPool_create(10);

    kpThreadPool_pushTask(p, &__print, (void *)100);
    kpThreadPool_destroy(p);
    return 0;
}
