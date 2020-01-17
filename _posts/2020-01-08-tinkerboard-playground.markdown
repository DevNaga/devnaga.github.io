---
layout: post
title:  "I bought a Tinkerboard"
date:   2019-1-19 23:26:00
categories: technology,cryptography, c, yocto, android
---

I finally got hold of an SBC [Tinkerboard](https://www.amazon.in/Asus-Tinker-90MB0QY1-M0UAY0-Socket-Motherboard/dp/B06VSBVQWS/ref=sr_1_1?keywords=tinker+board&qid=1579266011&sr=8-1) for my personal projects and learning purposes.

![Fullbox](https://raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/IMG-2730.JPG)

![Hardware](https://raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/IMG-2731.JPG)


The specs:
==========


```c
CPU : Rockchip RK3288 Cortex-A17 quad-core SoC

GPU: ARM Mali-T764 GPU

Video: Up to 4K video support

RAM: 2GB DDR3

Wifi: 802.11 b/g/n Wi-Fi

BT: Bluetooth 4.0

USB i/o: 4 x USB 2.0 ports

CSI: 15-pin MIPI CSI slot

GPIO: 40-pin GPIO header

Power: micro USB port for power
```


Somethings to consider:
========================

This one is a cute looking board at the size of your computer mouse, but is a big beast at power at its size. This is much more of an edge computing hardware (might be an entrant in there - i am sure its no match to the Intel UDOOs or the Huawei's NPUs) 

1. So this is not a 64 bit chip. Its A17 from the family of ARM-V7a architecture (not V8). It runs at 1.8 GHZ and has frequency scaling support for power savings. I would gonna run it at fullspeed if i have had a good cooling.
2. I am thinking to use it for object detection, recogintion and post processing tasks - so i'd require something like this GPU for starters.
3. Not gonna use monitor anyways - once i have ssh service and ethernet statically configured.
4. RAM - i would really require it for power and RAM hungry applications and services i have.
5. Wifi / BT - not really thinking an use case for my project at this moment.
6. No usecase for USBs - i will only use one for debugging if i connect a monitor to it.
7. GPIO - may be, i have to think.
8. Power - obviously and i have to find a better power adapter.


The power rating the Tinkerboard manual required was 5V/2A. The USB power cables give out at 0.8 mA and most of Samsung older Mobile phone power adaptors give out around 0.7 mA. So i did get many hickups and sudden device restarts after a GPU initialization process.
A quick fix to this issue is to use your powerbank ! Yes ! (or may be if you have a power adaptor that support 2A current - generally Motoroals do)

Powerbanks typically comes up with a output rating of 5V/2A and my powerbank (Protronics - [Amazon](https://www.amazon.in/Portronics-POR-289-5000mAH-Lithium-Polymer/dp/B07PGYFLTQ/ref=sr_1_3?keywords=portronics+power&qid=1579265953&sr=8-3)) just outputs that.

I did not use / not created a serial cable yet with the GPIO on the hardware so i have used a monitor to display out the console log.

Flashing:
==========

1. unzip the downloaded image

2. attach your SDCARD to your laptop 

3. check the `dmesg` and look for device mounts (`sda`, `sdb` ..)

4. if there are partitions already on the SDCARD, then delete them using `fdisk /dev/sdx`.

5. save the changes and unplug and plugin the SDCARD back.

6. Now perform the dd `dd if=2019XXXX-TinkerOS.img of=/dev/sdx`. Make sure you are not doing `dd` on your hard drive :-)


The Firstboot:
===============

So a first boot looks pretty slow. I have been booting from the SD card and not using the eMMC yet. A look of the device boot with TinkerOS is below.

![TinkerOS](https://raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/IMG-2738.JPG)

Its easy to get hold of TinkerOS (A simple google made me download the packages [here](https://www.asus.com/uk/Single-Board-Computer/Tinker-Board/HelpDesk_Download/))


Android:
=========

The Flashing of Android is same as the above. 

A version Android Nougat (?) on this and it seem pretty decent look.

![TinkerOS](https://raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/IMG-2733.JPG)

NextSteps:
==========

Running gcc multicore build causes a lot of CPU load. The version of `cmake` on the `TinkerOS` seem a bit too old. So i downloaded `cmake` and started building with `make -j4`. The temperature quickly shoot up from 40 to 53 and then to 60 in few more mins. 

My project required a lot more opensource tooling and libraries and thus i figured to switch it to one of the following.

1. Yocto
2. Armbian

I am currently checking out the [Armbian](https://www.armbian.com/tinkerboard/) and i will put some more screenshots here.

Armbian looked pretty decent and the packages and libs i required were latest as well.

Since it does not have a UI, i have to manually configure the wifi adaptor to connect to my Access point.

So here's the procedure:

```bash
1. `sudo wpa_passphrase <your access point> <your access point password> | sudo tee /etc/wpa_supplicant.conf`
2. `sudo wpa_supplicant -c /etc/wpa_supplicant.conf -i wlan0`
```

check `iwconfig` see if the device is connected. Now, for the ip address run `dhclient -i wlan0`.


I would still have to try the Yocto to have a dedicated build on my 3 year old Surface Book-1 (Yes, running linux). I will update soon with my progress on Yocto.
