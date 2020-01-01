---
layout: post
title:  "Using of Wireless ioctls in C"
date:   2019-12-20 23:26:00
categories: wireless, linux, programming
---

This is a short tutorial on wireless ioctls. Following ioctls will be covered below with code snippets and a working example.

```c
SIOCGIWNAME - gets wireless mode (IEEE 802.11abgn etc)
SIOCGIWESSID - gets access point (AP) name
SIOCGIWFREQ - get wireless frequency
SIOCGIWRATE - gets AP rate
SIOCGIWAP - get AP mac address
SIOCGIWNICK - get nickname
```

The include header `/usr/include/linux/wireless.h` contain these ioctl names and expose a data structure called `iwreq`. It is as follows.

```c
struct iwreq {
    union {
        char ifrn_name[IFNAMSIZ];
    } ifr_ifrn;
    union iwreq_data u;
};

```

so Most of the ioctl queries require us fill the `ifrn_name`. To some, a memory addres must be provided.

Create a socket to perform most of the ioctls. It can be created as follows.

```c
int sock;

sock = socket(AF_INET, SOCK_DGRAM, 0);
```

1. Getting wireless mode: `SIOCGIWNAME`

    1. Fill in the name 
    2. Perform ioctl

    Below is a short example.

    ```c
    struct iwreq iw;
    int ret;

    memset(&iw, 0, sizeof(iw));
    strcpy(iw.ifr_name, "wlan0");
    ret = ioctl(sock, SIOCGIWNAME, &iw);
    if (ret < 0) {
        return -1;
    }

    printf("name: %s\n", iw.u.name);

    ```

2. Getting AP name : `SIOCGIWESSID`

3. Getting Wireless frequency : `SIOCGIWFREQ`

4. Getting AP Rate : `SIOCGIWRATE`

5. Getting AP Mac: `SIOCGIWAP`

6. Getting AP Nickname: `SIOCGIWNICK`


