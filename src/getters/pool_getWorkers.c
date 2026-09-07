#include "kronkpool/macros/optimization.h"
#include <stddef.h>
#include "../pool.h"

KP_API
size_t kpThreadPool_getWorkers(
    kpThreadPool * pool
)
{
    if (!pool) {
        return 0;
    }
    return pool->workers;
}
