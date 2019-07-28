---
layout: post
title:  "Safe and Realtime software program pattern"
date:   2019-06-1 23:26:00
categories: language, c++
---

TLDR, This post is more about how to write software that's definitive, has a (soft) realtime behavior, and can be applied anywhere in the software domain that use C++ as the programming language.


### 1. definitive

The definitive mean most importantly, the following.

1. memory resources
2. cpu time
3. hardware resource access

**1. memory resources**

Always consider that, calling `new`, `malloc`, `calloc`, `realloc` mean a penalty to the program. Avoid calling them as much as possible. The reason: page faults. Operating systems (such as linux, BSD) that provide the allocation API, most of the time tend to use virtual memory, and if that memory is touched by the program (accessed read / write), it then tries to find a real-page in the physical memory (i.e. RAM). This means, the program is kept into "wait" state till the OS finds one free physical page. Thus, the program does not satisfy a basic realtime behavior.

this does not mean that one **shall not** always call the above memory allocation functions. But, call them in places such as,

1. program initialization (setup code)

    1. allocate structures you are using in the beginning, do not re-allocate them again
    2. allocate class objects if required, do not reallocate them again

We all know that structures and classes form the basis of a software program.

this also mean that, the rate of `nullptr` dereferences will be less if not none. See below.

2. scan for APIs and standard libraries, when using APIs that call internally the memory allocation functions, and replace them with ones that does not perform memory allocation.

3. it is not always possible to find out how much memory to use, when the program waits for data on a socket, pipe. Fit it based on your maximum packet size that your function / thread is handling to receive the data on this socket.

**Note:**
Remember that when using TCP connections, aggregation can happen, thus causing the `recv` system call to return more than expected bytes. Beware about this situation. Do not use TCP unless otherwise a connection state is requried by the server (such as a key exchange server, authorization server, or attestation for ex). For all the data communications, use UDP.

4. consider using stack allocations, but never exceed more than 8KB of the stack. Never change system defined limits for stack size (Aka, do not change stack size to be 8M). If your program requires that, then you should go back, alter the allocation and use dynamic memory allocation as specified in 1.

5. When using data structures (lists, queues and hash tables), keep an upper limit on how many elements that the particular section of your program needs (based on its inputs) and pre-allocate them. When removing elements (from lists, queues and hash tables), do not call free. Instead use a boolean variable called `in_use` and set it to `true` when using it, and set it to `false` when not using it.


**2. cpu time**

1. Make sure your process does not call `sleep`. this lets OS schedule the process out and may even keep it in swap area. Use events to wait for some resources / data. This is directly related to the realtime behavior of the software.

2. do not write loops that cannot be stopped. If your process needs to run forever, it may use an infinite loop. However, most of the software does not have to use them. An infinite loop that's not managed with events, makes it execute always on CPU and does not let any other process use it, causing it to high system load. This does not mean that you are allowed to use `sleep` in loops.

3. use system defined managed loop methods, `range based for` in C++ is one example. This handles the condition and increment while you only have to do initialization. Even if you forget condition and increment (which leads to infnite loops), using the `range based for` will not let you cause an infinite loop.

4. intiailize once - use it forever. Most of the parameters (memory, fifo, file descriptor, etc) in the code does not need initialization always, it could be initialized once and could be reused forever. Everytime when your code initializes, it wastes CPU.


**3. hardware resource access**

1. If the hardware is slower in providing the data in time domain, you could always wake your receive function / thread on an event that you register in kernel.

2. If the hardware is faster in providing the data than you consume, interrupts does not help, as you spend your time in handling interrupts than reading data. Hardware help can be taken, an interrupt can be set to queue the data in the hardware ring buffers, a program can wake up periodically, deque and transfer it to the memory from hardware.

3. Use DMA if the hardware provides the capability. This concept is directly related to the realtime behavior.

4. When accessing via DMA, make sure to hold a lock (PCI hardware lock for ex), a producer can enter and could corrupt the memory.

5. Always monitor hardware faults. Some hardware could provide fault information via some special protocol, use it and always monitor via the interface. Take necessary action (fallback, shutdown, erase) when a fault is being reported.

### 2. (soft) realtime behavior

1. **NEVER** call `sleep`, `std::chrono::this_thread::sleep_for`. It makes your program to context switch. More over, it again depends on the OS scheduler - it may swap out the program causing it to take time to load again. Rather use OS supported event based system calls to wait on.

2. **Reduce the use of locks** if there is a function that must execute at almost all the times, that must satisfy the execution time (such as 3microsecs execution time) limits always. Lock only the portions of the code that needs (such as retreiving data from a shared queue instance for ex).

3. **Avoid using signals**, signals are asynchronous in behavior. They could come at anytime when you are using them for things such as timers (`timer_create`). This means that the signal handler can preempt the currently executing code. If your program is threaded and uses signals, this makes a whole new level of complicated case causing unknown / undefined deadlock situations.

3. **Avoid excessive queueing** of data. This causes delays in retrieving data, adds more pipes to follow, makes data stale and lose freshness.

4. **NEVER** change priority of threads unless otherwise you absolutely know by practical proofs. Reason: priority inversion, causing every other threads to go deadlock where there is a shared data between them.

5. Always validate logical behavior of your important function / code. If a logical flow is altered to what is being expected, perform an action such as failure trigger, log the behavior and shutdown the program. Reasons could be many: program is not validated for this input, or a program is being made to step into the particular section of the code (code injection / stack / program counter is being altered).

6. Always monitor deadline behavior of your important function / code. Failure to meet deadline can lead to either fallback / system restart / stop the execution. While the system restart comes under a hard realtime behavior (letting the secondary identical hardware module to takeover and perform the main software objectives).

Some systems does not allow a system restart all the times when there is a failure, but in certain conditions.

Any particular deadline miss must be logged and all the inputs causing this behavior.

7. Tie your serious work thread to one core. This makes OS not to choose the core for thread to execute under some certain conditions. Keeping your thread to one core, removes the swapping of the program between the core and the cache as well.

8. Always align your data to the cache line. The more cache hits your program can perform, the less it require to be able to access from the RAM. RAM is always slower to access than cache.


Apart from these, tools that are available at hand, could be used for achieving most (if not all of) of above methods.

1. compilers can determine if you have looping problem, undefined references, stack usage, optimisation etc.
2. code coverage methods can effectively determine, what part of code is covered and exactly what functions and with lines.

