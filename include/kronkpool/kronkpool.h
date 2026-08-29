/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Header and public API
*/
#ifndef KRONKPOOL_H
    #define KRONKPOOL_H
    #include "kronkpool/macros/optimization.h"
    #include <sys/types.h>    

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Forward declaration of struct kronkpool_threadpool_s
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronkpool_threadpool_s kpThreadPool;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
KP_API kpThreadPool* kpThreadPool_create(ssize_t concurrency);

// TODO: Documentation
KP_API void kpThreadPool_destroy(kpThreadPool* pool);

// TODO: Documentation
KP_API int kpThreadPool_pushTask(kpThreadPool *pool, void *(*task)(void *), void *data);

// TODO: Documentation
KP_API size_t kpThreadPool_getWorkers(kpThreadPool * p);

#endif /* KRONKPOOL_H */
