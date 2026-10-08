# Kronkpool

Kronkpool is a simple thread pool written in C, using a custom queue implementation and `pthread` (Win32 threads on
Windows).

## Features

- Multithreading: split tasks between workers.
- Queue-based task scheduling.
- Shared and static library builds.
- Linux and Windows (MSVC, MinGW).
- Optional unit tests from the root CMake project.

## Build

Kronkpool uses CMake.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

This generates `libkronkpool.a` and `libkronkpool.so` in `build/`.

On Windows, the same commands work with MSVC (Visual Studio 2022 17.5 or later, for `<stdatomic.h>`), with
`--config Release` to build. They give `kronkpool_static.lib`, and `kronkpool.dll` with its import library
`kronkpool.lib`. MinGW gives `libkronkpool.a` and `libkronkpool.dll`.

```bash
cmake --install build
```

### Unit Tests

Unit tests are enabled by default from the top-level CMake file. A smoke test (`tests/smoke/`) runs on every
platform, against the static and the shared library. The unit tests (`tests/units/`) use
[kronklab](https://github.com/kronkorp/kronklab), pinned to a commit, which runs each test in a `fork()`: they are not
built on Windows.

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
