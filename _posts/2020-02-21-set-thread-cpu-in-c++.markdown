---
layout: post
title:  "Schedule thread to a particular CPU in C++"
date:   2020-02-21 23:26:00
categories: technology,cryptography, c, yocto, android
---

Threads are implemented in C++11 as `std::thread`. Threads can be created as,

```cpp
void thread_func(void)
{
}

void main()
{
    std::thread t(thread_func);
}
```

however, thread mechanism dont provide capability to schedule the thread on a particular CPU.

The `std::thread` provide mechansim to access the native `pthread_t` type as `std::thread::native_handle()`. Using this, the thread can be tied to a particular CPU.

In `pthread`, the following APIs can be used to tie the thread to a particular CPU.

```cpp
int pthread_setaffinity_np(pthread_t tid, int size, cpu_set_t *cpuset);
```


The below API extends the functionality of `std::thread` namespace - adding the feature for tieing the thread on particular CPU.

```cpp
#include <iostream>
#include <thread>
#include <pthread.h>

int schedule_thread_on(int cpu_no, std::thread *this_thread)
{
    cpu_set_t cpuset;
    int ret;

    CPU_ZERO(&cpuset);
    CPU_SET(cpu_no, &cpuset);
    ret = pthread_setaffinity_np(this_thread->native_handle(), sizeof(cpu_set_t), &cpuset);
    if (ret < 0) {
        return -1;
    }

    return 0;
}

```

