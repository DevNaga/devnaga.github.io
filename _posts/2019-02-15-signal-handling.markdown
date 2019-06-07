---
layout: post
title:  "Signal handling"
date:   2019-06-01 23:26:00
categories: language, c++
---

We know that the signal handling in POSIX based operating system is handled via a system call. They are usually asynchronous in nature.

The available system calls,

| system call | description |
|-------------|-------------|
| `signal` | basic signal handling function |
| `sigaction` | more detailed way of handling signals |
| `sigwait` | wait till a signal occurs |
| `sigprocmask` | block / unblock signals |


We, however do have some more syscalls under linux, we cannot be using them when we develop an OS abstraction layer. Lets look from POSIX point of view.

the prototype of `signal` syscall looks below.

```c

int signal(int signum, void (*signal_handler)(int));

```

Lets write a simple example on the `signal` system call.

```cpp

#include <iostream>
#include <thread>
#include <signal.h>

void signal_handler(int s)
{
    std::cout << "signal occured "<< s << std::endl;
}

int main()
{
    signal(SIGINT, signal_handler);

    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


```

compiling it with `g++ signals.cc -std=c++11` gives us a program that registers signal handler and sleep for ever.

Now, when we do `ctrl+c` we get the signal being handled in the `signal_handler` because we get the print.

The `ctrl+c` being the `SIGINT`. More about signal numbers in `signal.h`.

Now, lets think that the code in `while(1)` doing a very important job, that is setting important data members (shared data) for example, and a signal might as well cause the particular program execution to stop and run the signal handler. OK.

Now, the signal handler code starts touching the data member, and now we have race. The `sigprocmask` exactly does provide the solution to this.

Lets look at the `sigprocmask` below.

```c

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

```

How is one of the following:

```c

SIG_BLOCK
SIG_UNBLOCK

```

the `set` is of `sigset_t` which can be set with the `sigfillset` - fills all the signals (although `SIGKILL` cannot be masked and triggers a default program termination), thus the call to the `sigprocmask` involve the following.

```cpp

sigset_t mask;

sigfillset(&mask);

sigprocmask(SIG_BLOCK, &mask, nullptr);

.. critical section ..

sigprocmask(SIG_UNBLOCK, &mask, nullptr);

```


