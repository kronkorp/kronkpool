/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Threads, mutexes and condition variables, the same way on every platform.
*/
#ifndef KRONKPOOL_PLATFORM_THREAD_H
    #define KRONKPOOL_PLATFORM_THREAD_H

    // NOTE: The one place that knows which thread API each platform has. The
    //       functions below are written once per platform, in
    //       src/platform/<platform>/ (CMake builds the one of the target)
    #ifdef _WIN32
        #include <windows.h>
        typedef HANDLE             kpThread;
        typedef SRWLOCK            kpMutex;
        typedef CONDITION_VARIABLE kpCond;
    #else
        #include <pthread.h>
        typedef pthread_t          kpThread;
        typedef pthread_mutex_t    kpMutex;
        typedef pthread_cond_t     kpCond;
    #endif /* _WIN32 */

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Start a thread that runs routine(arg)
 *
 * @return  0, or -1
 */
///////////////////////////////////////////////////////////////////////////////
int kpThread_create(kpThread *thread, void *(*routine)(void *), void *arg);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Wait for a thread to end, and release it
 */
///////////////////////////////////////////////////////////////////////////////
void kpThread_join(kpThread thread);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Get the number of processors online
 *
 * @return  The number, or -1 if it is not known
 */
///////////////////////////////////////////////////////////////////////////////
long kpThread_cpuCount(void);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   A mutex: made, destroyed, locked and unlocked
 *
 * @note    kpMutex_init returns 0, or -1
 */
///////////////////////////////////////////////////////////////////////////////
int kpMutex_init(kpMutex *mutex);
void kpMutex_destroy(kpMutex *mutex);
void kpMutex_lock(kpMutex *mutex);
void kpMutex_unlock(kpMutex *mutex);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   A condition variable, waited on with its mutex locked
 *
 * @note    kpCond_init returns 0, or -1
 */
///////////////////////////////////////////////////////////////////////////////
int kpCond_init(kpCond *cond);
void kpCond_destroy(kpCond *cond);
void kpCond_wait(kpCond *cond, kpMutex *mutex);
void kpCond_signal(kpCond *cond);
void kpCond_broadcast(kpCond *cond);

#endif /* KRONKPOOL_PLATFORM_THREAD_H */
