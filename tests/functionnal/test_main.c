#include "kronkpool/kronkpool.h"

int main(void)
{
    kpThreadPool *p = kpThreadPool_create(10);

    kpThreadPool_destroy(p);
    return 0;
}
