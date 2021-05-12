---
layout: post
title:  "Sending UDP packets with VLAN in Linux"
date:   2020-12-18 23:26:00
categories: wireless, linux, programming
---

VLANs can be used to logically divide the network into different groups. It is one of the useful security defense mechanism against known attacks also.

VLAN header is described in IEEE 802.1Q: It will look as follows:

```bash
|--------------------------------------------------|
| TPID      | Priority  |  CFI      | VID          |
|-----------|-----------|-----------|--------------|
|<-2 bytes->|<- 3 bits->|<- 1 bit ->|<- 12 bits  ->|
```

TPID is generally set to 0x8100 denoting VLAN ethertype.
After VID is followed by the ethertype of the next header. In case of ipv4 it is 0x0800 and in case of ipv6 it is 0x86dd.


A few things about VLANs is that,


1. VLAN network must be created on both sender and receiver within the same VLAN ID.
2. VLAN in linux is created as virtual network interfaces  appear in `ifconfig` command.
3. When communication over ip is required on the VLAN interface, An ip address must be required.

For example i have the following setup between the two networks.


```bash

|-------------------|                         |-----------------|
| Raspberry PI 4    |<----------------------->| Linux Laptop    |
|  (Vlan ID 4)      |                         | (Vlan  ID 4)    |
| IP: 192.168.1.4   |                         | IP: 192.168.1.6 |
|-------------------|                         |-----------------|

```

1. Raspberry PI4 is running linux has a  VLAN ID of 4. It also has an ip address on the VLAN interface as 192.168.1.4.
2. Linux laptop is running linux has a VLAN ID of 4. It also has an ip address on the VLAN interface as 192.168.1.6.

Below command is used to create vlan interface on linux:

```bash
ip link add link eth0 name eth0.4 type vlan id 4
```


