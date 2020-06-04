---
layout: post
title:  "TRNG on ATSAM3XEA (Arudino Due reference board)"
date:   2020-06-04 14:00:00
categories: cryptography
---

This article is about using the TRNG module within the ATSAM3XEA as i couldn't find an implementation online. if you are TLDR, read to the "Using TRNG" part.

A bit introduction to the ATSAM3XEA.

Introduction
=============

ATSAM3XEA is an ARM Cortex-M3 based microcontroller. It has 512 KB Flash and 32KB SRAM. I have been trying to catch hold of Cortex-M based microcontrolers and this one comes very cheap [Amazon](https://www.amazon.in/Robocraze-Arduino-Due-Robotics-Projects/dp/B07GVBWV78/ref=sr_1_1?dchild=1&keywords=arduino+due&qid=1591287202&sr=8-1) (Prices might have gone up .. amazon ffs) when i bought it.

The Arduino due comes with two micro USB ports. One Native USB and another Programmable. Either one can be used to program in Arduino IDE.

For custom board setup, just connect it and the Arduino IDE detects it automatically and just install the ATSAM3XEA package.

For programming connect the programming side with Micro USB and otherside to the computer.

More on the ATSAM3XEA is [here](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf).

on-chip TRNG
=============

ATSAM3XEA has onchip TRNG. From the reference manual, The True Random Number Generator (TRNG) passes the American NIST Special Publication 800-22 and DiehardRandom Tests Suites.


Some characteristics of this TRNG.

1. Passed NIST Special Publication 800-22 Tests Suite
2. Passed Diehard Random Tests Suite
3. Provides a 32-bit Random Number Every 84 Clock Cycles

The TRNG generation sequence is shown in the below diagram.

![TRNG_Generation](https:////raw.githubusercontent.com/DevNaga/devnaga.github.io/blob/master/_posts/TRNG_sequence.png)






