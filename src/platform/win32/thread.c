/*
** FREE PROJECT, 2026
** KRONKPOOL
** File description:
** Threads on Windows (SRW locks and condition variables, Vista and later).
*/
#include "../thread.h"
#include <process.h>
#include <stdint.h>
#include <stdlib.h>

// NOTE: _beginthreadex wants an "unsigned __stdcall" routine: kpThread_start
//       is that one, and calls the routine it was given
typedef struct kronkpool_thread_start_s {
    void *(*routine)(void *);
    void *arg;
} kpThreadStart;

static unsigned __stdcall kpThread_start(void *data)
{
    kpThreadStart start = *(kpThreadStart *)data;

    free(data);
    start.routine(start.arg);
    return 0;
}

int kpThread_create(kpThread *thread, void *(*routine)(void *), void *arg)
{
    kpThreadStart *start = malloc(sizeof(kpThreadStart));
    uintptr_t handle;

    if (!start) {
        return -1;
    }
    start->routine = routine;
    start->arg = arg;
    handle = _beginthreadex(NULL, 0, &kpThread_start, start, 0, NULL);
    if (handle == 0) {
        free(start);
        return -1;
    }
    *thread = (HANDLE)handle;
    return 0;
}

void kpThread_join(kpThread thread)
{
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
}

long kpThread_cpuCount(void)
{
    SYSTEM_INFO info;

    GetSystemInfo(&info);
    return (long)info.dwNumberOfProcessors;
}

// NOTE: An SRW lock and a condition variable hold nothing to release
int kpMutex_init(kpMutex *mutex)
{
    InitializeSRWLock(mutex);
    return 0;
}

void kpMutex_destroy(kpMutex *mutex)
{
    (void)mutex;
}

void kpMutex_lock(kpMutex *mutex)
{
    AcquireSRWLockExclusive(mutex);
}

void kpMutex_unlock(kpMutex *mutex)
{
    ReleaseSRWLockExclusive(mutex);
}

int kpCond_init(kpCond *cond)
{
    InitializeConditionVariable(cond);
    return 0;
}

void kpCond_destroy(kpCond *cond)
{
    (void)cond;
}

void kpCond_wait(kpCond *cond, kpMutex *mutex)
{
    SleepConditionVariableSRW(cond, mutex, INFINITE, 0);
}

void kpCond_signal(kpCond *cond)
{
    WakeConditionVariable(cond);
}

void kpCond_broadcast(kpCond *cond)
{
    WakeAllConditionVariable(cond);
}
