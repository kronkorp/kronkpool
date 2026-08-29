# Kronkpool

*Kronkpool* is a simple threadpool written in C, using custom queue implementation and **pthread**.

## Features

- **Multithreading**: Split tasks between differents *workers* (threads).

## Installation

### Using CMake

```bash
mkdir build
cd build/
cmake ..
make
```

This will generate `libkronkpool.a` and `libkronkpool.so`. You can now install with

```bash
sudo make install
```

## Quick Start

Here is a simple example of how to use Kronkflow:

```c
#include <pthread.h>
#include <kronkpool/kronkpool.h>
#include <stdio.h>

// Your task handler
static void *my_task(void *arg)
{
    printf("Task executed in thread [%zu] with arg [%p]\n", pthread_self());
}s

int main(void)
{
    // Create a threadpool with [HARDWARE_CONCURRENCY] workers
    kpThreadPool *p = kpThreadPool_create(-1);

    // Get the number of workers
    printf("Threadpool has %zu workers\n", kpThreadPool_getWorkers(p));

    // Push a task
    kpThreadPool_pushTask(p, &my_task, (void *)10);

    // Destroy the pool
    kpThreadPool_destroy(p);
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
