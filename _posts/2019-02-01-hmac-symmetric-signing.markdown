---
layout: post
title:  "HMAC signing and verify"
date:   2019-1-19 23:26:00
categories: technology,cryptography
---

HMAC provides symmetric signatures and verification. The most prominent hash algorithms when using HMAC are SHA256 and SHA-3.

This means the same key is used for both ends, sign and verify. The hash must be transmitted alongside the message (usually ASN.1 encoded) so the receiver with the same key HMAC's the input and verifies that the message is authentic.

HMAC is useful in places where there is necessity for faster authentication of message. ECC, is very good for signing and verification but it takes a while for ECC to generate signatures, HMAC is way faster in doing the signatures.

Since there is only one key used for both sign and verify this must be kept secret. Similarly with the ECC, the private key must always be kept secret.

I am going to use OpenSSL for HMAC sign and verify. Here are some details.

### key generation

Its a symmetric key. Key length preferably above 128 bits - 16 bytes.

A simple call to `RAND_bytes` generates a key.

```c

uint8_t key[16];
int ret;

ret = RAND_bytes(key, sizeof(key));
if (ret < 0) {
    return -1;
}

write(key_file_fd, key, sizeof(key));

```

### signing

Signing is very simple. Here, these are the steps.

1. load HMAC key - with `EVP_PKEY_new_mac_key` with `EVP_PKEY_HMAC` as the first argument
2. create an MD_CTX
3. initialize the digest
4. update and finalize the digest

### Verification

Verification is very simple. Here, these are the steps.

1. load HMAC key - with `EVP_PKEY_new_mac_key` with `EVP_PKEY_HMAC` as the first argument
2. create an MD_CTX
3. initialize the digest
4. update and finalize the digest
5. compare the input hmac signature with the finalised digest. Comparison yields 0 if both digests are matched, if not, the signature is treated invalid, and then the message is dropped. For comparison `CRYPTO_memcmp` is used.

### Full example

Below is a C++ class implementation of the HMAC wrapper around the openssl.

compile it with `-lssl -lcrypto`.

<script src="https://gist.github.com/DevNaga/96e18e88f3d3742cdce00679ca4875ac.js"></script>
