---
layout: post
title:  "watch for more crypto-howto on my github profile"
date:   2019-3-20 23:26:00
categories: technology, c, linux, system programming, cryptography, openssl
---

 ( I am not a native English Speaker, and i am sure there are some grammatical errors below, i apologize. )

I am working on an opensource project of my own called [Edge Computing SDK](https://github.com/devoslab/edge_sdk), which in general is a middleware library that has everything from files, sockets, system call abstractions, threads and queuing, data structures to cryptographic routines.

So, this project made me focus more on things that i have not learnt, (aka. cryptography, distributed computing) and thus i have (and i am still) spent more of my free time into writing the crypto agnostics layer that support any / all underlying cryptographic library abstractions.

Right now, i have supported openssl, most widely used crypto library to almost able to do from hash algorithms to TLS communications. I have yet to complete on DTLS, X509, ASN.1, PKCS formats. So i have yet to learn much more of these before i could write up an abstract interface to the openssl.

There is another library, called wolfssl (most known in the embedded computing domain), i have added little bit of support to that library as well, only hashing and key generation as of now.

So, i also have started a small repository on crypto-howto for openssl (as of today) [here](https://github.com/DevNaga/openssl_manual). Reason is that, over the time, i found it very difficult to understand the openssl API and usage because of a limited documentation available in the man pages of libssl-doc.

Feel free to follow me / watch the repository for more updates on the coming content. The content and the source code of the repository are opensource and if you want to copy it, you **must** follow the MIT licensing guidelines before you do so.


