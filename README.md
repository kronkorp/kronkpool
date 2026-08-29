# Kronkpool

Kronkpool is a simple thread pool written in C, using a custom queue implementation and `pthread`.

## Features

- Multithreading: split tasks between workers.
- Queue-based task scheduling.
- Shared and static library builds.
- Optional unit tests from the root CMake project.

## Build

Kronkpool uses CMake.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

This generates `libkronkpool.a` and `libkronkpool.so` in `build/`.

```bash
cmake --install build
```

### Unit Tests

Unit tests are enabled by default from the top-level CMake file.

```bash
cmake -S . -B build -DKRONKPOOL_BUILD_TESTS=ON
cmake --build build -j
ctest --test-dir build
```

To skip test targets, configure with `-DKRONKPOOL_BUILD_TESTS=OFF`.

## Quick Start

Here is a simple example of how to use Kronkpool:

```c
#include <pthread.h>
#include <kronkpool/kronkpool.h>
#include <stdio.h>

static void *my_task(void *arg)
{
    printf("Task executed in thread [%zu] with arg [%p]\n", pthread_self(), arg);
    return NULL;
}

int main(void)
{
    kpThreadPool *p = kpThreadPool_create(-1);

    printf("Threadpool has %zu workers\n", kpThreadPool_getWorkers(p));

    kpThreadPool_pushTask(p, &my_task, (void *)10);

    kpThreadPool_destroy(p);
    return 0;
}
```

## Public API

- `kpThreadPool_create(ssize_t concurrency)` creates a pool.
- `kpThreadPool_destroy(kpThreadPool *pool)` stops workers and releases resources.
- `kpThreadPool_pushTask(kpThreadPool *pool, void *(*task)(void *), void *data)` queues a task.
- `kpThreadPool_getWorkers(kpThreadPool *pool)` returns the configured worker count.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
