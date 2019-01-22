---
layout: post
title:  "Programmatically Generate RSA keys"
date:   2019-1-22 23:26:00
categories: cryptography
---

On the path towards learning OpenSSL fully and to complete some of my own [EDGEOS](https://github.com/devnaga/edgemw) project features, i have encountered RSA Signatures.

To perform any asymmetric signing or verification, it would require keys. RSA being the PKI, involve generating keypairs, Public and private.

OpenSSL library's EVP interface provides helper functions to generate keypairs.

to do this include `openssl/evp.h` and `openssl/rsa.h`.

Below are the full steps involved.

### create new EVP_KEY_CTX for RSA.

```c
EVP_PKEY_CTX *ctx;

ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
if (!ctx) {
    ERR_print_errors_fp(stderr);
    return ERR_FAILURE;
}

```

this creates a context that is used later in the keygen 

### setup keygen context

```c
int ret;

ret = EVP_PKEY_keygen_init(ctx);
if (ret != 1) {
    return ERR_FAILURE;
}

```

### set RSA keygen bits. RSA algorithm support key lengths of 1024, 2048, 3072 and 4096 bits.

```c
int ret;

ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048); // lets' generate for the 2048 bits
if (ret != 1) {
    return ERR_FAILURE;
}

```

### generate key

```c
EVP_PKEY *key;
int ret;

ret = EVP_PKEY_keygen(ctx, &key);
if (ret != 1) {
    return ERR_FAILURE;
}

```

If everything is ok, then the keys are generated in the key obj.

Writing to the file is as simple as using the PEM API. For this use `openssl/pem.h`

### Write private and public key

```c
int ret;
FILE *fp;

fp = fopen("rsa_2048_p.key", "w");
if (!fp) {
    return ERR_FAILURE;
}

ret = PEM_write_PrivateKey(fp, key, NULL, NULL, 0, NULL, NULL);
if (ret != 1) {
    return ERR_FAILURE;
}

fclose(fp);

fp = fopen("rsa_2048.key", "w");
if (!fp) {
    return ERR_FAILURE;
}

ret = PEM_write_PUBKEY(fp, key);
if (ret != 1) {
    return ERR_FAILURE;
}

fclose(fp);

```

Below is the full code. Compile with `-lssl -lcrypto`.

<script src="https://gist.github.com/DevNaga/e008a4540a80d232d4ca2a78a3b78603.js"></script>





