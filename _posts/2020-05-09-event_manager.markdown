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
2. more work queued / starvation / delayed execution - 



You can look at the demonstrative example design on the Event Manager implementation [here](http://devnaga.github.io/2019/gcd-fraemwork.html). I called it with name "Grand Central Dispatch" mimic'ing with the apple's GCD framework.




