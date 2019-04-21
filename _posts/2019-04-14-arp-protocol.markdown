---
layout: post
title:  "ARP protocol"
date:   2019-04-14 23:26:00
categories: networking
---

ARP is also called Address Resolution Protocol defined as [RFC ARP](https://tools.ietf.org/html/rfc826) as one of the internet standard.

The protocol is defined to let the machines know about each other's MAC addresses when communicating with each other.

**ARP header**


```

0                    2                     3             4                5
|--------------------|---------------------|-------------|----------------|
| hardware type      | protocol type       | hw addr len | proto addr len |
|-------------------------------------------------------------------------|
| op code            | source hardware address                            |
|-------------------------------------------------------------------------|
| src_hwaddr(2bytes) | source protocol address                            |
|-------------------------------------------------------------------------|
| destination hardware address                                            |
|-------------------------------------------------------------------------|
| destination protocol address             |
|------------------------------------------|

```

The ARP header described as,


```c

struct arp_header {
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hw_addr_len;
    uint8_t proto_addr_len;
    uint16_t op;
    uint8_t source_hwaddr[6];
    uint32_t source_proto_addr;
    uint8_t dest_hwaddr[6];
    uint32_t dest_proto_addr;
};

```

**protocol field lengths and description:**

1. The `hardware_type` set to 1 for ethernet.

2. The `protocol_type` is basically ethertype: for IPv4 it is set to 0x800.

3. The `hw_addr_len` is set to 6 and `proto_addr_len` is set to 4.

4. The `op` is one of the following.

| op | description |
|----|-------------|
| 1  | ARP request |
| 2  | ARP reply   |
| 3  | RARP request |
| 4  | RARP reply  |
| 5  | DRARP request |
| 6  | DRARP reply |
| 7  | InARP request |
| 8  | InARP reply |


5. When sending ARP request the op code must be 1. The `source_hwaddr` must be the mac of the source. The `source_proto_addr` is the ip address of the source. The `dest_hwaddr` is set to broadcast address and the `dest_proto_addr` is the ip of the destination.

6. If the destination is alive, it will send an ARP reply to the host and the host receives an ARP reply with op code set to 2.

7. When the destination sends an ARP reply, it sets its hardware address and ip address in the `source_hwaddr` and `source_proto_addr` fields. The `dest_hwaddr` and `dest_proto_addr` is set to the sender's (that is the sender that has sent the ARP request packet) hardware and the ip addresses.



