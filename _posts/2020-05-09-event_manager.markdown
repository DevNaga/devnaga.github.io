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




You can look at the demonstrative example design on the Event Manager implementation [here](http://devnaga.github.io/2019/gcd-fraemwork.html). I called it with name "Grand Central Dispatch" mimic'ing with the apple's GCD framework.




