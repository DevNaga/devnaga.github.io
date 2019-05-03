---
layout: post
title:  "Version 1 of Butler"
date:   2019-03-15 23:26:00
categories: networking, cryptography, IOT, embedded, security
---

Version 1 of [Butler](https://github.com/devnaga/butler) released.


[Butler](https://github.com/DevNaga/butler) is a high performance middleware stack for embedded devices that talk to the cloud for various applications. These type of devices usually are **IoT** devices and **Edge** devices ranging with hardware characteristics of 52MB RAM to 4 GBs of RAM.

I will describe below the software features and details of the butler software stack.

Below show a diagram of the butler middleware.

![](https://github.com/DevNaga/butler/blob/master/butler%20services1.png?raw=true)


The architecture contains a butler middleware framework library below abstracting the OS and posix System calls. The users of this layer are the `storage`, `monitoring` and `DLM` modules.

Version 1 does not really have the OS abstraction and it only support Linux as of this release.

Storage, Monitoring and DLM are right now not supported and in very early stages. I am writing software for them.

Right now, the first version available only contain the Butler library.

The butler middleware is designed to abstract the OS system calls and provide helper library functions to aid in writing code quick.

The lower library also called the name butler.

Below are the components of the butler library.

![](https://github.com/DevNaga/butler/blob/master/butler.png?raw=true)

The library is divided into 6 core parts.

1. system call abstraction layer
2. framework
3. algorithms
4. configuration parsers
5. GPS abstraction
6. crypto abstraction

System call abstraction is used by many software services and programs to be compatible across a wide range of OSes. Right now, i have access to linux and i have created a version of abstraction for linux. In future may be BSD and QNX (if i could buy the license) is my plan. 


## System call abstraction

The system call abstraction supports the following

1. UART
2. Shared memory
3. File system API, Abstraction and Monitoring
4. Process utility
5. Signal utility
6. Network interface abstraction
7. ioctl abstraction
8. POSIX message queue abstraction
9. Pipe and fifo abstraction
10. scheduler utility


Out of the 10 interfaces, release 1 does not contain the following.

1. UART


The Shared memory provide an abstraction above the `mmap` and `shm_` system calls and allow easier API to use without worrying underlying implementation.

The file system API abstract the POSIX `open`, `close`, `read` and `write` system calls, allowing easier API use not worry about how and what modes to open a file and what system calls to use for reading / deleting directories.

The process utility right now at the early stage and it only support basic utilities to check if you are a root and getting the hostname.

The Signal utility right now at the early stage as well, it only support block and unblock of signals abstracting the `sigprocmask`.

The Network interface abstraction is developed in detail with the following API.

1. unmanaged and managed tcp/udp/unix domain client and server API
2. raw socket interface and abstraction
3. socket options

The Main takeaway for these API abstractions is the ease of programming that they allow.

For example, writing a udp server is as simple as calling

```c
char *ip = "127.0.0.1";
int port = 2414;
int sock;

sock = btlr_create_udp_sock(ip, port);
if (sock < 0) {
    return -1;
}

printf("<1> created server %d\n", sock);

```

one does not have to worry about the underlying OS for getting a network mac address or an ip. Simply call,

```c
int ret;
uint8_t hwaddr[6];
const char *ifname = "eth0";

ret = btlr_get_hwaddr(ifname, hwaddr);
if (ret < 0) {
    return -1;
}

printf("<1> mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
                hwaddr[0], hwaddr[1], hwaddr[2],
                hwaddr[3], hwaddr[4], hwaddr[5]);

```

or an ip address say,

```c
int ret;
char ipaddr[20];
const char *ifname = "eth0";

ret = btlr_get_net_ipaddr(ifname, ipaddr, sizeof(ipaddr));
if (ret < 0) {
    return -1;
}

printf("<1> ipaddr: %s\n", ipaddr);

```


## Framework

The framework provide the following features:

1. thread pool
2. event loop
3. logging
4. resource manager

The thread pool allow and support creation of threads (Posix Threads) and scheduling the work on them.

Pool allows creation of n number of threads and let each thread wait for the task. The master thread can schedule a task on a particular thread. More than one job can be queued to a thread at a time but they will be completed in sequential manner, that is only the current job execution completes, the next queued job will not be executed.

Right now, the thread pool only allow creation of one worker thread. Plans to make it the following.

1. dispatch queue is created
2. make threads based on number of cores.
3. Algorithm to queue threads (execution based on thread execution)

The current implementation also support tieing the threads with the particular core.

Event loop is another framework to schedule timers and wake up on sockets. The timer resolution is 1 microsecond and sockets can be registered and events can be called upon as callbacks.

The logging framework is under development to support remote logging as well (to a laptop or to cloud). The current logging framework allow writing the string on the screen with color to allow for debug and analysis.

The resource manager is a resource controller that once a valid resource limit is set, it does not allow if the limit exceed the configured value of threshold.

## Algorithms

Following algorithms are supported.

1. Lists
2. Doubly lists
3. Stacks
4. Queues
5. Hash tables
6. Circular lists
7. Static lists (for embedded devices)
8. Geometrical algorithms

The items from 1 to 7 are algorithms generally used in various programs for queueing / storing / retrieval and gathering purposes. The geometrical algorithms support only the base distance and geometrical functions.


## Configuration parsers

The configuration parser right now support a "variable = value" type of configuration. Plans are toward support of `.ini` files and may be in future more commonly used file formats.


## GPS abstraction

The GPS abstraction right now is not available. The following support is thought of.

1. NMEA 1083
2. Ublox - M8, F9
3. Teseo III, V


## Crypto abstraction


The crypto abstraction provide interfaces above `OpenSSL` and `WolfSSL`. Right now the support for `OpenSSL` API is almost available and `WolfSSL` is in early stage.

The following crypto operations are supported.

1. Hashing - Md5, Sha1, Sha256, Sha384, Sha512 and RipeMD160
2. Encryption and Decryption - AES CBC, GCM in 128, 192 and 256
3. Public key signatures and verification - RSA, ECC. RSA key lengths 2048 and 4096 and ECC key lengths 256 are supported.
4. key generation - symmetric, RSA key and ECC key
5. TLS v1.2 API support basic

support is missing for `.csr` generation, validation. It is missing support for `X509` API.

Examples of the crypto API usage is described here.

**Hashing**

```cpp
uint8_t data[] = "example crypto library";
uint8_t hash_sha2[32];
int ret;

ret = btlr_crypto_sha256(data, sizeof(data), hash_sha2);
if (ret < 0) {
    return;
}

btlr_hexdump("sha2", hash_sha2, sizeof(hash_sha2));
```

## Building and installing


1. clone: `git clone https://github.com/DevNaga/butler`
2. checkout: `git checkout v1`
3. install:
    1. `./build.sh make_release`.

            This creates `butler_release-v0.1-alpha.tar`

    2. `sudo ./install_butler.sh butler_release-v0.1-alpha.tar`


The installation writes the library in `/usr/lib/` and includes in `/usr/include/btlr/`.


Right now i have tested this software on my laptop - 4 core Intel Core2 and 4GB of RAM and on Raspberry PI 3. I did not test on Raspberry PI full part of the software release still.





