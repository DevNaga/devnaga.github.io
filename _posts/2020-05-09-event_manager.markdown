---
layout: post
title:  "Event Manager Framework"
date:   2020-05-09 14:00:00
categories: cryptography
---

In many systems, some programs intend to run forever and work on the i/o events and do some actions regarding the i/o, use memory and act on the events from various other processes and do some timely actions.

So here we broadly categorize them as the following:

1. timer
2. socket (i/o) or file descriptor (such as data buses or system descriptors)
3. signals (stop signals for example)
4. threads - ability to utilize multi-core and run things in parallel if the hardware allow


To describe them in detail,

1. A system job must be run periodically utilizing the timer event. For example, send a message periodically, report a device status to the cloud for ex.
2. A socket connection event, ex: http socket, mqtt socket, or a internet socket or local file descriptor (process socket) has data that the process needs to read upon
3. A signal event, where some action required to be performed on the received signal
4. Threads - parallelize work on multiple cores, utilize multiple cores to scale up work and increase responsiveness of the task execution

Every modern day Service Oriented Architecture would have one or more of these patterns repeating in every single software that is being written. This means a framework must be created to serve this repeating and identical behavior of the service.

![Event Manager](https:////raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/event_manager.jpeg)

Above diagram is a generalized framework implementing the 4 possible SOA functionalities (timers, sockets, signals or thread pools)

The framework implemented as a "Event_Manager" class. Has a below set of public member functions:

```cpp

1. Create_Timer_Event
2. Create_Socket_Event
3. Create_Signal_Event
4. Execute_Work
5. Loop
6. Terminate

```

The underlying "Event_Manager" class implements a list of Timer base, Socket base, Signal base and a Thread pool.

Each Timer base, is a list of timers which inturn is the timeout and a callback function to be called when there is an event. This pattern is repeated with the Socket and Signal bases.

The Framework would wait for the timer, socket and signal events and inturn validates against the events. Upon arrival of the event, the callback is called upon from the Framework.

The procedure of calling the callback would take multiple approaches.

1. call from the framework directly - the callback function shouldn't be consuming a lot of work. In most cases, the execution times of the order of microseconds wouldn't cause so much of harder problem. But it depends really on the
usecases: some use cases such as Banking, high frequency trading might do multiple things at a time when the callbacks are called (such as from the internet socket) and thus tend to execute them in a thread. Some applications such as
text messages, wouldn't consume much of resource than pushing the same text message on to a UI thread and sleeping back.

2. call from a thread - event manager schedules the callback on a dedicated thread and continues waiting for the rest of the events. callback can be executed in parallel and could take some order of execution interval.

In either of the cases, we have limitations and some possible fixes.

1. drifts / latencies / reduced throughput and responsiveness - when called from framework directly and if the callbacks consume more time than is expecting. This inturn mean that validating the time it takes for a callback to execute
and flag the offending callback and stop the program so that the code could be properly altered later point.
2. more work queued / starvation / delayed execution - if there are limited threads within the pool, each might have a queue of pending work to be executed. In such instances, we will see the delayed execution caused by the wait time
and starvation if thread pool work schedule is not implemented properly.

Generally to solve the work queue and delayed execution, the scheduling policy and scaling is used.

1. scheduling policy - a round robin is a simpler solution to waive off the starvation a bit but would not suffice.
So generally, each work that is being executed must also have an associated work cost (weight). When scheduling the work item on particular thread, the weights must be checked to see how "loaded"
the thread is and consider into queueing into the thread will lowest possible load.

2. scaling - this would be an on-demand scaling - for generally cpu intensive applications (video processing, streaming, network data etc) - more threads might need to handle the incoming data work
in parallel. Generally a threadpool would reserve a particular amount of threads based off of the number of cores the processor has (some has 1 core some has as many as 4). sometimes, the number
of threads to handle the incoming requests are not enough and will see a responsiveness and performance issues. To overcome it, the threadpool manager, shall continously measure the load on each
thread and validate under a particular load if the given hard limits are meeting (time of execution to complete, latency in the results of execution). Under conditions of not able to meet such
deadlines, the threadpool manager, must instanciate more threads and keep monitoring the performance.


The "Loop" and "Terminate" are the simplest form of all the other API.

The Loop would act as a waitor for all the above events and acts on them.

1. An event could be a timer event and the action could be calling the corresponding timer callback or scheduling it on the dedicated thread.
2. An event could be a signal and socket event and the action could be similar to the timer event handling

To break the loop, "Terminate" is used. At any point in the code, calling "Event_Manager::Terminate()" activates the Exit calls which inturn automatically invokes the destructors and stops the Event Manager.

The advantages of the Event Manager being that it makes the SOA software more of event based, it generalizes the framework for every single pattern that require above basic i/o's. This is not something that's totally new,
it is being existed in many places and many times - for example [libevent](https://libevent.org/) is one of the most popular event framework for C/ C++ software, used in from embedded systems to a large scale systems.


You can look at the demonstrative only (and not meant for direct use !!) example design on the Event Manager implementation [here](http://devnaga.github.io/2019/gcd-fraemwork.html). I called it with name "Grand Central Dispatch" mimic'ing with the apple's GCD framework.




