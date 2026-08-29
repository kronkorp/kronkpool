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

typedef void *(*kpThreadPoolHandler)(void *);

typedef struct kronkpool_thread_task_s {

    kpThreadPoolHandler handler;
    void*               data;

} kpThreadTask;

// TODO: Doc
typedef struct kronkpool_threadpool_s {

    atomic_size_t   pendings;  //!< The number of task pendings
    atomic_size_t   runnings;  //!< The number of thread up and runnings
    atomic_size_t   workers;   //!< The number of threads
    pthread_t*      threads;   //!< The threads (array)
    pthread_cond_t  cond;      //!< The conditionnal variable
    pthread_mutex_t mutex;     //!< Mutex
    atomic_bool     stop;      //!< Does the pool should stop
    // Queue of tasks ? (dynamic array, ring buffer, linked list)?
    queue_t         queue;     //!< Queue (implementation in "src/queue/")

} kpThreadPool;

// TODO: Documentation
void *kpThreadPool_routine(void *arg);

#endif /* KRONKPOOL_PRIVATE_H */
