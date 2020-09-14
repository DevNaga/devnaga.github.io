---
layout: post
title:  "Networking and Security: ARP intro"
date:   2020-09-14 23:26:00
categories: networking
---

**ARP** is short name for Address Resolution Protocol. It is used to get mac addresses based on known ip address. Tools like `ping` generally trigger the ARP frames. ARP is required to know the destination mac address so to send an receive any message. ARP operates at the L2 layer.

ARP header looks like the following. ![ARP_Header](https://raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/ARP_Header.png)

Below is the ARP data structure.


```c


struct arp_header {
    hardware_type       hw_type;
    uint16_t            proto_type;
    uint8_t             hw_addr_len;
    uint8_t             protocol_addr_len;
    operation           op;
    uint8_t             sender_hwaddr[6]; // mac address
    uint8_t             sender_proto_addr[4]; // ipv4 address
    uint8_t             target_hwaddr[6]; // mac address
    uint8_t             target_proto_addr[4]; // ipv4 address
};

```


ARP is initiated based on following conditions:

1. a new device is connected in the network. - device tries to reach the default gateway
2. a packet needs to be sent to the device or to another device and that L2 destination mac address is not available to fill.
3. periodic arps - initiated by the router or the default gateway to learn the network devices status

So this provides us a basic information of IP to mac and vice versa translations. The networking stack keeps an ARP table of learnt addresses so it can set the corresponding mac address in the L2 Ethernet frame.

An example of the ARP table below is as follows.

```c

struct l2_arp_table {
    uint8_t source_hwaddr[6];
    uint32_t source_ipaddr;
    uint8_t target_hwaddr[6];
    uint32_t target_ipaddr;
    bool has_response;      // has a valid response
    timestamp last_updated; // last updated timestamp
};

```

Below is some code example i have done for ARP packet serialization and deserialization.

**arp.h**:

```c

/**
 * @brief - ARP serialize and deserialize
 * 
 * @copyright - All rights reserved 2020-present Devendra Naga (devendra.aaru@outlook.com)
 */
namespace auto_os::network {

enum class hardware_type : int {
    eEthernet,
    eIeee802,
    eArcnet,
    eFrameRelay,
    eAtm,
    eHdlc,
    eFiberChannel,
    eAtm2,
    eSerialLine,
};

enum class operation : int {
    eReserved = 0,
    eArpRequest,
    eArpReply,
    eRarpRequest,
    eRarpReply,
    eDrarpRequest,
    eDrarpReply,
    eDrarpError,
    eInarpRequest,
    eInarpReply,
};

struct arp_header {
    hardware_type       hw_type;
    uint16_t            proto_type;
    uint8_t             hw_addr_len;
    uint8_t             protocol_addr_len;
    operation           op;
    uint8_t             sender_hwaddr[6]; // mac address
    uint8_t             sender_proto_addr[4]; // ipv4 address
    uint8_t             target_hwaddr[6]; // mac address
    uint8_t             target_proto_addr[4]; // ipv4 address
};

class Arp_Layer {
    public:
        explicit Arp_Layer() = default;
        ~Arp_Layer() = default;

        int Serialize(uint8_t *buf, size_t buf_size, size_t &off, arp_header *arp_h);
        int Deserialize(uint8_t *buf, size_t buf_size, size_t &off, arp_header *arp_h);
        void Pretty_Print(arp_header *arp_h);
};

```

**arp.cc**

```cpp

/**
 * @brief - ARP serialize and deserialize
 * 
 * @copyright - All rights reserved 2020-present Devendra Naga (devendra.aaru@outlook.com)
 */
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include <autonet_arp.h>
#include <helpers.h>

namespace auto_os::network {

int Arp_Layer::Serialize(uint8_t *buf, size_t buf_size, size_t &off, arp_header *arp_h)
{
    uint16_t data;

    data = auto_os::lib::bswap16b(static_cast<int>(arp_h->hw_type));
    memcpy(buf + off, &data, sizeof(data));
    off += 2;

    data = auto_os::lib::bswap16b(arp_h->proto_type);
    memcpy(buf + off, &data, sizeof(data));
    off += 2;

    buf[off] = arp_h->hw_addr_len;
    off ++;

    buf[off] = arp_h->protocol_addr_len;
    off ++;

    data = auto_os::lib::bswap16b(static_cast<int>(arp_h->op));
    memcpy(buf + off, &data, sizeof(data));
    off += 2;

    memcpy(buf + off, arp_h->sender_hwaddr, 6);
    off += 6;

    memcpy(buf + off, arp_h->sender_proto_addr, 4);
    off += 4;

    memcpy(buf + off, arp_h->target_hwaddr, 6);
    off += 6;

    memcpy(buf + off, arp_h->target_proto_addr, 4);
    off += 4;

    return -1;
}

int Arp_Layer::Deserialize(uint8_t *buf, size_t buf_size, size_t &off, arp_header *arp_h)
{
    uint16_t data;

    memcpy(&data, buf + off, 2);
    off += 2;

    arp_h->hw_type = static_cast<hardware_type>(auto_os::lib::bswap16b(data));

    memcpy(&arp_h->proto_type, buf + off, 2);
    off += 2;

    arp_h->proto_type = auto_os::lib::bswap16b(arp_h->proto_type);

    arp_h->hw_addr_len = buf[off];
    off ++;

    arp_h->protocol_addr_len = buf[off];
    off ++;

    memcpy(&data, buf + off, 2);
    off += 2;

    arp_h->op = static_cast<operation>(auto_os::lib::bswap16b(data));

    memcpy(arp_h->sender_hwaddr, buf + off, 6);
    off += 6;

    memcpy(arp_h->sender_proto_addr, buf + off, 4);
    off += 4;

    memcpy(arp_h->target_hwaddr, buf + off, 6);
    off += 6;

    memcpy(arp_h->target_proto_addr, buf + off, 4);
    off += 4;

    return off;
}

static const std::string get_arp_hw_type(hardware_type type)
{
    switch (type) {
        case hardware_type::eArcnet:
            return "ARCNet";
        case hardware_type::eAtm2:
        case hardware_type::eAtm:
            return "ATM";
        case hardware_type::eEthernet:
            return "Ethernet";
        case hardware_type::eFiberChannel:
            return "Fibre Channel";
        case hardware_type::eFrameRelay:
            return "Frame Relay";
        case hardware_type::eHdlc:
            return "HDLC";
        case hardware_type::eIeee802:
            return "IEEE 802";
        case hardware_type::eSerialLine:
            return "Serial";
        default:
            return "Unknown";
    }

    return "Unknown";
}

static const std::string get_protocol_type(uint16_t protocol_type)
{
    switch (protocol_type) {
        case 0x0800:
            return "IPv4";
        default:
            "Unknown";
    }

    return "Unknown";
}

static const std::string get_opcode(operation op)
{
    switch (op) {
        case operation::eArpRequest:
            return "ARP Request";
        case operation::eDrarpError:
            return "DRARP Error";
        case operation::eDrarpReply:
            return "DRARP Reply";
        case operation::eDrarpRequest:
            return "DRARP Request";
        case operation::eInarpReply:
            return "INARP Reply";
        case operation::eInarpRequest:
            return "INARP Request";
        case operation::eRarpReply:
            return "RARP Reply";
        case operation::eRarpRequest:
            return "RARP Request";
        case operation::eArpReply:
            return "ARP Reply";
        default:
            return "Unknown";
    }

    return "Unknown";
}

void Arp_Layer::Pretty_Print(arp_header *arp_h)
{
    fprintf(stderr, "Arp: {\n");
    fprintf(stderr, "\t hw_type : %s\n", get_arp_hw_type(arp_h->hw_type).c_str());
    fprintf(stderr, "\t protocol type : %s\n", get_protocol_type(arp_h->proto_type).c_str());
    fprintf(stderr, "\t hw addr len : %d\n", arp_h->hw_addr_len);
    fprintf(stderr, "\t protocol addr len : %d\n", arp_h->protocol_addr_len);
    fprintf(stderr, "\t operation : %s\n", get_opcode(arp_h->op).c_str());
    fprintf(stderr, "\t sender hw addr : [%02x:%02x:%02x:%02x:%02x:%02x]\n",
                            arp_h->sender_hwaddr[0],
                            arp_h->sender_hwaddr[1],
                            arp_h->sender_hwaddr[2],
                            arp_h->sender_hwaddr[3],
                            arp_h->sender_hwaddr[4],
                            arp_h->sender_hwaddr[5]);
    fprintf(stderr, "\t sender proto addr : [%02d.%02d.%02d.%02d]\n",
                            arp_h->sender_proto_addr[0],
                            arp_h->sender_proto_addr[1],
                            arp_h->sender_proto_addr[2],
                            arp_h->sender_proto_addr[3]);
    fprintf(stderr, "\t target hw addr : [%02x:%02x:%02x:%02x:%02x:%02x]\n",
                            arp_h->target_hwaddr[0],
                            arp_h->target_hwaddr[1],
                            arp_h->target_hwaddr[2],
                            arp_h->target_hwaddr[3],
                            arp_h->target_hwaddr[4],
                            arp_h->target_hwaddr[5]);
    fprintf(stderr, "\t target proto addr : [%02d.%02d.%02d.%02d]\n",
                            arp_h->target_proto_addr[0],
                            arp_h->target_proto_addr[1],
                            arp_h->target_proto_addr[2],
                            arp_h->target_proto_addr[3]);
    fprintf(stderr, "}\n");
}

}


```


Below is an example run that i have captured with my TP-Link router, while sending a test `ping`.

```c

Arp: {
	 hw_type : IEEE 802
	 protocol type : IPv4
	 hw addr len : 6
	 protocol addr len : 4
	 operation : ARP Request
	 sender hw addr : [d4:6a:6a:d5:45:4f]
	 sender proto addr : [192.168.00.107]
	 target hw addr : [00:00:00:00:00:00]
	 target proto addr : [192.168.00.01]
}
Arp: {
	 hw_type : IEEE 802
	 protocol type : IPv4
	 hw addr len : 6
	 protocol addr len : 4
	 operation : ARP Reply
	 sender hw addr : [70:4f:57:60:46:55]
	 sender proto addr : [192.168.00.01]
	 target hw addr : [d4:6a:6a:d5:45:4f]
	 target proto addr : [192.168.00.107]
}

```

the problem here is that a device with sufficient tcp/ip capabilities with a network card, can try to fake in the ARP replies acting as a man-in-the-mdidle faking the actual sender. This is also called ARP spoof.

A simpler way to avoid such spoofs is to use static ARP tables. In general it works in networks where the network configuration never changes. Such as for ex: Automotive networks, data centers (but may not apply for dynamically changing VM instances).

A network filter can be used to perform much more detection capabilities. That will be discussed later in the coming section.

Few of the filter parameters i will be considering next in the implementation of ARP in the network filter:

**deny rules:**

```c

1. if (arp.protocol type != IPv4) && if (arp.hw_addr_len != 6) && if (arp.protocol_addr_len != 4)
2. (if (arp.op == ARP_Req) || if (arp_op == ARP_Reply) ) && ( if (arp.source_hw_addr == 00-00-00-00-00-00) ||
                               if (arp.source_hw_addr == FF-FF-FF-FF-FF) ||
                               if (arp.source_hw_addr_pattern == 01-80-c2)
                               if (arp.source_proto_addr == 0.0.0.0) )
3. if (arp.op == ARP_Reply) && !prev_arp_req_rx
4. if (arp.op == ARP_Reply) && (arp_entry.last_updated - timeout < 0)
5. if neither (arp.op == ARP_Req) nor (arp.op == ARP_Reply)

```
