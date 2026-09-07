/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Private struct definition
*/
#ifndef KRONKPOOL_PRIVATE_H
    #define KRONKPOOL_PRIVATE_H
    #include <stdatomic.h>
    #include <pthread.h>
    #include <kronkpool/macros/types.h>
    #include "queue/queue.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  type declaration to make development easier and source code clearer
 */
///////////////////////////////////////////////////////////////////////////////
typedef void *(*kpThreadPoolHandler)(void *);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronkpool_thread_task_s
 *
 * @brief  A simple struct with handler and arg to give with handler
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronkpool_thread_task_s {

    kpThreadPoolHandler handler;  //!< The task ptr (function ptr) to execute
    void*               data;     //!< The data to give to handler

} kpThreadTask;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronkpool_threadpool_s
 *
 * @brief  Manage all members needed to run the threadpool
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronkpool_threadpool_s {

    atomic_size_t   pendings;  //!< The number of task pendings
    atomic_size_t   runnings;  //!< The number of thread up and runnings
    atomic_size_t   workers;   //!< The number of threads
    pthread_t*      threads;   //!< The threads (array)
    pthread_cond_t  cond;      //!< The conditionnal variable
    pthread_mutex_t mutex;     //!< Mutex
    atomic_bool     stop;      //!< Does the pool should stop
    // FIXME: Maybe check for a better queue implementation
    queue_t         queue;     //!< Queue (implementation in "src/queue/")

} kpThreadPool;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief  The routine of the threadpool, give uppon creation
 *
 * @param arg  The arg (the threadpool)
 * @return     The result (always NULL)
 */
///////////////////////////////////////////////////////////////////////////////
void *kpThreadPool_routine(void *arg);
///////////////////////////////////////////////////////////////////////////////

#endif /* KRONKPOOL_PRIVATE_H */
