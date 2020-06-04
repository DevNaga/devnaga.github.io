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

The TRNG generation sequence is shown in the below diagram. This diagram is taken out of the ATSAM3XEA manual.


![TRNG_Generation](https://raw.githubusercontent.com/DevNaga/devnaga.github.io/master/_posts/TRNG_sequence.png)


The following steps are required to perform enablement of TRNG.

1. Enable TRNG clock
2. Enable TRNG_CR -> Write "RNG" and set the last bit (bit 0)
3. Enable TRNG_IER -> Interrupt enable register
4. check the status of interrupt setting in TRNG_IMR.
5. in a loop, perform the remaining steps.
6. check if TRNG_ISR is set, if yes read TRNG_ODATA


Using TRNG
==========

make the following settings,

```cpp
unsigned int *TRNG_CR = (unsigned int *)0x400BC000;
unsigned int *TRNG_IER = (unsigned int *)0x400BC010;
unsigned int *TRNG_IDR = (unsigned int *)0x400BC014;
unsigned int *TRNG_IMR = (unsigned int *)0x400BC018;
unsigned int *TRNG_ISR = (unsigned int *)0x400BC01C;
unsigned int *TRNG_ODATA = (unsigned int *)0x400BC050;

// enable peripheral clock..
pmc_enable_periph_clk(ID_TRNG);

// write RNG and last bit set it
*TRNG_CR = 0x524e4701;

// enable interrupt
*TRNG_IER = 0x01;

// validate if we have set the interrupt
if (*TRNG_IMR != 0x01) {
    return -1; // or call while(1) and abort program / flash LED etc
}

bool trng_read = false;

unsigned int bytes = 0;

while (1) {
    int retries = 0;

    // check for ISR
    if (*TRNG_ISR & 0x01) {
        // copy TRNG data
        bytes = *TRNG_ODATA;
        trng_read = true;
    }

    retries ++;
    if (retries > 100) {
        break;
    }
}

if (trng_read) {
    // write TRNG on to the console
    Serial.println("trng data: ");
    Serial.print(bytes);
    Serial.println();
}

```

With this thing in mind, we can create a key generation program, and C++ 11 for abstraction with multiple types of the TRNGs. This may be too much in general, there could be only one if not many of these TRNG devices.

Below program help describe the Key Generation demonstration with the help of TRNG in C++ 11. You would require an Arduino Due (or compatible board with ATSAM3XEA), a USB to micro USB cable and Arduino IDE to compile and re-flash.

<script src="https://gist.github.com/DevNaga/fb1fa3b862093d01d378b4be4f96b2bd.js"></script>
