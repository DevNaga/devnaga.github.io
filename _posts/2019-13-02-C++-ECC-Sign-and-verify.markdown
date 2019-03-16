---
layout: post
title:  "ECC sign and verify with OpenSSL in C++"
date:   2019-1-21 23:26:00
categories: technology, c++
---

ECC (Elliptic Curve Cryptography) is another asymmetric cryptograhy with the likes of the RSA.

ECC has the advantage of smaller key sizes. Unlike the RSA, ECC keys are small, however it is somewhat of CPU intensive when it comes to signing a message and verifying them on an embedded hardware.

In this one, only the EC portion of openssl will be detailed, and i will not be explaining the theoretical concepts of ECC.


For this we require `EC`, `SHA`, `rand`, `PEM` and `EVP` includes. Aka,

```cpp
#include <openssl/evp.h>
#include <openssl/ecdsa.h>
#include <openssl/ec.h>
#include <openssl/sha.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

```

and the regular headers,

```cpp
#include <openssl/err.h>
#include <openssl/conf.h>

```

### key generation

OpenSSL has the key generation functions for EC. EC engine supports many curves, doing 

```bash
openssl ecparam -list_curves
```

produce a lot of supported curves such as the following,

```bash
  secp112r1 : SECG/WTLS curve over a 112 bit prime field
  secp112r2 : SECG curve over a 112 bit prime field
  secp128r1 : SECG curve over a 128 bit prime field
  secp128r2 : SECG curve over a 128 bit prime field
  secp160k1 : SECG curve over a 160 bit prime field
  secp160r1 : SECG curve over a 160 bit prime field
  secp256k1 : SECG curve over a 256 bit prime field
  secp384r1 : NIST/SECG curve over a 384 bit prime field
  secp521r1 : NIST/SECG curve over a 521 bit prime field
  prime192v1: NIST/X9.62/SECG curve over a 192 bit prime field
  prime192v2: X9.62 curve over a 192 bit prime field
  prime192v3: X9.62 curve over a 192 bit prime field
  prime239v1: X9.62 curve over a 239 bit prime field
  prime239v2: X9.62 curve over a 239 bit prime field
  prime239v3: X9.62 curve over a 239 bit prime field
  brainpoolP160r1: RFC 5639 curve over a 160 bit prime field
  brainpoolP160t1: RFC 5639 curve over a 160 bit prime field
  brainpoolP192r1: RFC 5639 curve over a 192 bit prime field
  brainpoolP192t1: RFC 5639 curve over a 192 bit prime field
  brainpoolP224r1: RFC 5639 curve over a 224 bit prime field
  brainpoolP224t1: RFC 5639 curve over a 224 bit prime field
  brainpoolP256r1: RFC 5639 curve over a 256 bit prime field
  brainpoolP256t1: RFC 5639 curve over a 256 bit prime field
  brainpoolP320r1: RFC 5639 curve over a 320 bit prime field
  brainpoolP320t1: RFC 5639 curve over a 320 bit prime field
  brainpoolP384r1: RFC 5639 curve over a 384 bit prime field
  brainpoolP384t1: RFC 5639 curve over a 384 bit prime field
  brainpoolP512r1: RFC 5639 curve over a 512 bit prime field
  brainpoolP512t1: RFC 5639 curve over a 512 bit prime field
```

each curve name in the EC is denoted with `NID_` macro defined in `openssl/obj_mac.h`. One does not have to include this header file to use the curve name.

Usually, the generated private key is stored safe and the public key is distributed for the intended receivers. So this means the program must load in the already generated private keys as well and support key generation once.

The `EC_KEY` contains the context associated with the key generation. A new curve is configured for the `EC_KEY` context with the use of `EC_KEY_new_by_curve_name`.

```cpp
EC_KEY *key;
int nid = NID_brainpoolp256r1;

key = EC_KEY_new_by_curve_name(nid);

```

We are not configuring the curve parameters and so there wont' be any use of `_bio_` functions in this example.

Here are the set of steps that's followed

1. Generating the keys is done via the use of `EC_KEY_generate_key` and
2. validating it is done using `EC_KEY_check_key`.
3. Finally the keys are written using the `PEM_` functions, `PEM_write_EC_PUBKEY` and `PEM_write_ECPrivateKey`.

A simple example of such function is described below,

<script src="https://gist.github.com/DevNaga/4f4c7a08f0783a52e9c661f3d4196050.js"></script>


Below example shows the loading of public key,

Here are the set of steps that's followed

1. load in the public key with `PEM_read_EC_PUBKEY`
2. create new EVP_PKEY object for verification key
3. assing the public key to the new EVP_PKEY object

<script src="https://gist.github.com/DevNaga/2259d8d239b526ed661864f97ed835ea.js"></script>


same steps are followed for the load of private key, Below example shows the loading of private key,


<script src="https://gist.github.com/DevNaga/58df06084a8763463ebc8f89124fe259.js"></script>


**validating the keys:**

the `openssl` command line can be used to validate the generated keys,

This is what i get, when doing (for the pubkey),

```bash
openssl ec -in ec_brainpool_256r1.pub -pubin -text

```

**output:**

```bash
read EC key
Private-Key: (256 bit)
pub: 
    04:6d:59:08:25:eb:58:d6:96:8f:24:bb:d6:fd:7a:
    84:ab:86:14:40:8c:d5:33:15:bf:86:a9:52:b8:6c:
    40:16:c6:3f:2a:68:b3:40:ac:92:89:c2:d4:76:a8:
    24:65:5b:d2:2c:95:53:15:78:5d:fd:b7:ec:1f:b8:
    c1:e7:b1:cb:1b
Field Type: prime-field
Prime:
    00:a9:fb:57:db:a1:ee:a9:bc:3e:66:0a:90:9d:83:
    8d:72:6e:3b:f6:23:d5:26:20:28:20:13:48:1d:1f:
    6e:53:77
A:   
    7d:5a:09:75:fc:2c:30:57:ee:f6:75:30:41:7a:ff:
    e7:fb:80:55:c1:26:dc:5c:6c:e9:4a:4b:44:f3:30:
    b5:d9
B:   
    26:dc:5c:6c:e9:4a:4b:44:f3:30:b5:d9:bb:d7:7c:
    bf:95:84:16:29:5c:f7:e1:ce:6b:cc:dc:18:ff:8c:
    07:b6
Generator (uncompressed):
    04:8b:d2:ae:b9:cb:7e:57:cb:2c:4b:48:2f:fc:81:
    b7:af:b9:de:27:e1:e3:bd:23:c2:3a:44:53:bd:9a:
    ce:32:62:54:7e:f8:35:c3:da:c4:fd:97:f8:46:1a:
    14:61:1d:c9:c2:77:45:13:2d:ed:8e:54:5c:1d:54:
    c7:2f:04:69:97
Order: 
    00:a9:fb:57:db:a1:ee:a9:bc:3e:66:0a:90:9d:83:
    8d:71:8c:39:7a:a3:b5:61:a6:f7:90:1e:0e:82:97:
    48:56:a7
Cofactor:  1 (0x1)
writing EC key
-----BEGIN PUBLIC KEY-----
MIIBMzCB7AYHKoZIzj0CATCB4AIBATAsBgcqhkjOPQEBAiEAqftX26Huqbw+ZgqQ
nYONcm479iPVJiAoIBNIHR9uU3cwRAQgfVoJdfwsMFfu9nUwQXr/5/uAVcEm3Fxs
6UpLRPMwtdkEICbcXGzpSktE8zC12bvXfL+VhBYpXPfhzmvM3Bj/jAe2BEEEi9Ku
uct+V8ssS0gv/IG3r7neJ+HjvSPCOkRTvZrOMmJUfvg1w9rE/Zf4RhoUYR3JwndF
Ey3tjlRcHVTHLwRplwIhAKn7V9uh7qm8PmYKkJ2DjXGMOXqjtWGm95AeDoKXSFan
AgEBA0IABG1ZCCXrWNaWjyS71v16hKuGFECM1TMVv4apUrhsQBbGPypos0CskonC
1HaoJGVb0iyVUxV4Xf237B+4weexyxs=
-----END PUBLIC KEY-----

```

For the private key:

```bash
openssl ec -in ec_brainpool_256r1.pkey  -text

```


**output:**

```bash
read EC key
Private-Key: (256 bit)
priv:
    00:a1:a8:82:7c:2a:41:8c:0c:ea:15:8e:ef:d8:73:
    23:2d:5a:5d:76:8a:75:c0:0f:4e:06:36:60:0b:65:
    52:84:3e
pub: 
    04:6d:59:08:25:eb:58:d6:96:8f:24:bb:d6:fd:7a:
    84:ab:86:14:40:8c:d5:33:15:bf:86:a9:52:b8:6c:
    40:16:c6:3f:2a:68:b3:40:ac:92:89:c2:d4:76:a8:
    24:65:5b:d2:2c:95:53:15:78:5d:fd:b7:ec:1f:b8:
    c1:e7:b1:cb:1b
Field Type: prime-field
Prime:
    00:a9:fb:57:db:a1:ee:a9:bc:3e:66:0a:90:9d:83:
    8d:72:6e:3b:f6:23:d5:26:20:28:20:13:48:1d:1f:
    6e:53:77
A:   
    7d:5a:09:75:fc:2c:30:57:ee:f6:75:30:41:7a:ff:
    e7:fb:80:55:c1:26:dc:5c:6c:e9:4a:4b:44:f3:30:
    b5:d9
B:   
    26:dc:5c:6c:e9:4a:4b:44:f3:30:b5:d9:bb:d7:7c:
    bf:95:84:16:29:5c:f7:e1:ce:6b:cc:dc:18:ff:8c:
    07:b6
Generator (uncompressed):
    04:8b:d2:ae:b9:cb:7e:57:cb:2c:4b:48:2f:fc:81:
    b7:af:b9:de:27:e1:e3:bd:23:c2:3a:44:53:bd:9a:
    ce:32:62:54:7e:f8:35:c3:da:c4:fd:97:f8:46:1a:
    14:61:1d:c9:c2:77:45:13:2d:ed:8e:54:5c:1d:54:
    c7:2f:04:69:97
Order: 
    00:a9:fb:57:db:a1:ee:a9:bc:3e:66:0a:90:9d:83:
    8d:71:8c:39:7a:a3:b5:61:a6:f7:90:1e:0e:82:97:
    48:56:a7
Cofactor:  1 (0x1)
writing EC key
-----BEGIN EC PRIVATE KEY-----
MIIBUQIBAQQgoaiCfCpBjAzqFY7v2HMjLVpddop1wA9OBjZgC2VShD6ggeMwgeAC
AQEwLAYHKoZIzj0BAQIhAKn7V9uh7qm8PmYKkJ2DjXJuO/Yj1SYgKCATSB0fblN3
MEQEIH1aCXX8LDBX7vZ1MEF6/+f7gFXBJtxcbOlKS0TzMLXZBCAm3Fxs6UpLRPMw
tdm713y/lYQWKVz34c5rzNwY/4wHtgRBBIvSrrnLflfLLEtIL/yBt6+53ifh470j
wjpEU72azjJiVH74NcPaxP2X+EYaFGEdycJ3RRMt7Y5UXB1Uxy8EaZcCIQCp+1fb
oe6pvD5mCpCdg41xjDl6o7VhpveQHg6Cl0hWpwIBAaFEA0IABG1ZCCXrWNaWjyS7
1v16hKuGFECM1TMVv4apUrhsQBbGPypos0CskonC1HaoJGVb0iyVUxV4Xf237B+4
weexyxs=
-----END EC PRIVATE KEY-----

```


### signing

The private key is used to sign the message and public is used to verify the message. Thus, one should never share the private key.

Signing process involves the following steps:

1. get a valid `MD_CTX` pointer basing on the hash algorithm to be used to sign the message. `EVP_sha1` or above are useful.
2. create `EVP_MD_CTX`.
3. initialise the Digest signature  context via `EVP_DigestSignInit` and set the SHA to be used and along with the private key (see `load_privkey`).
4. Update the message to be signed via `EVP_DigestSignUpdate`
5. Finalise the signature length and the signature via `EVP_DigestSignFinal`.

Usuaully the `EVP_DigestSignFinal` is called without signature pointer to get the signature length so that the signature can be allocated. Here' i took the buffer large enough so that the call can be done once.

Below example performs the signing process.

<script src="https://gist.github.com/DevNaga/cfbe5b807ea80eccc926eb0fddfb4ab3.js"></script>



### verification

Verification process involves the following steps:

1. get a valid `MD_CTX` pointer similar to the signing process outlined above. But it must the same hash algorithm that's used in the signing process.
2. create `EVP_MD_CTX`.
3. initialise the Digest verification context via `EVP_DigestVerifyInit` and set the SHA to be used and along with the public key (see `load_pubkey`).
4. Update the message to be verified via `EVP_DigestVerifyUpdate`
5. Finalise the verification by passing the signature via `EVP_DigestVerifyFinal`.

Below example performs the verification process.

<script src="https://gist.github.com/DevNaga/d5678ed26520bc6847910ce9578188c1.js"></script>




### class implementation

The above functionality can be wrapped in a class with the following member functions.


```cpp

class ecc_base {
    public:
        ecc_base();
        ~ecc_base();
        
        // load public key
        int load_pubkey(std::string pubkey);
        
        // load private key
        int load_privkey(std::string privkey);
        
        // gen keys pub and priv with the given curve
        int generate_keys(std::string pubkeyfile, std::string privkeyfile, std::string curvename);
        
        // generate signature
        int sign(uint8_t *msg, size_t msglen, std::string sha_alg);
        
        // verify signature
        int verify(uint8_t *msg, size_t msglen, uint8_t *signature, size_t signature_len, std::string sha_alg);
        
    private:
    
        uint8_t signature[256];
        size_t signature_len;
};

```

Below is the implementation of the `ecc_base` class.

<script src="https://gist.github.com/DevNaga/98a940ee18e347d0acfd9fe5f453ec30.js"></script>





