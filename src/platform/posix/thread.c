/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Threads on POSIX (pthreads).
*/
#include "../thread.h"
#include <unistd.h>

int kpThread_create(kpThread *thread, void *(*routine)(void *), void *arg)
{
    return pthread_create(thread, NULL, routine, arg) == 0 ? 0 : -1;
}

void kpThread_join(kpThread thread)
{
    pthread_join(thread, NULL);
}

long kpThread_cpuCount(void)
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int kpMutex_init(kpMutex *mutex)
{
    return pthread_mutex_init(mutex, NULL) == 0 ? 0 : -1;
}

void kpMutex_destroy(kpMutex *mutex)
{
    pthread_mutex_destroy(mutex);
}

void kpMutex_lock(kpMutex *mutex)
{
    pthread_mutex_lock(mutex);
}

void kpMutex_unlock(kpMutex *mutex)
{
    pthread_mutex_unlock(mutex);
}

int kpCond_init(kpCond *cond)
{
    return pthread_cond_init(cond, NULL) == 0 ? 0 : -1;
}

void kpCond_destroy(kpCond *cond)
{
    pthread_cond_destroy(cond);
}

void kpCond_wait(kpCond *cond, kpMutex *mutex)
{
    pthread_cond_wait(cond, mutex);
}

void kpCond_signal(kpCond *cond)
{
    pthread_cond_signal(cond);
}

void kpCond_broadcast(kpCond *cond)
{
    pthread_cond_broadcast(cond);
}
