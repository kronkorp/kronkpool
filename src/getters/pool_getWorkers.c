#include "kronkpool/macros/optimization.h"
#include <stddef.h>
#include "../pool.h"

KP_API
size_t kpThreadPool_getWorkers(
    kpThreadPool * p
)
{
    if (!p) {
        return 0;
    }
    return p->workers;
}
