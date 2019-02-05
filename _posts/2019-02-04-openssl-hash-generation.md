---
layout: post
title:  "Generating hashes programmatically with OpenSSL EVP_ functions"
date:   2019-1-22 23:26:00
categories: cryptography
---

Well, this is another howto on generating the cryptographic hashes with the openssl (like as  many as wiki pages available in the internet about the openssl hash functions).

I currently have openssl 1.0.2g on my personal linux machine. 

Currently the `man EVP_sha` describe only till `sha512` and no details of `sha3` (well newer openssl seem to have contained it).

Considering that the `MD` algorithm is broken that is `MD2`, `MD4` and `MD5`. I wouldn't use the `EVP_` routines related to these.

However, it looks like `MD5` is still in use by most of the web downloads (where, it gives the md5sum of the file).


Let's create a class called `hash_class` in C++.

```cpp
class hash_class {
};

```

the class must support the following hash functions. `md5`, `sha`, `sha1`, `sha224`, `sha256`, `sha384`, `sha512`. The resulting class must have them as member functions. Thus,

```cpp
class hash_class {
    public:
        int generate_md5(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha1(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha224(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha256(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha384(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha512(uint8_t *msg, size_t msglen, uint8_t *digest);
 };
 ```
 
 The openssl supports the hash function types as a set of functions returning the pointer to the type `const EVP_MD`. The following are the hash functions
 
 ```cpp
 EVP_md5(); // for md5
 EVP_sha(); // for sha
 EVP_sha1(); // for sha1
 EVP_sha224(); // for sha224
 EVP_sha256(); // for sha256
 EVP_sha384(); // for sha384
 EVP_sha512(); // for sha512
 
 ```
 
 This means a generic private member function can be written that can accept the type `const EVP_MD *` and can perform the hashing.
 
 To perform the hashing, openssl supports `EVP_Digest*` functions such as the following.
 
 ```cpp
 int EVP_DigestInit(EVP_MD_CTX *ctx, const EVP_MD *type);
 int EVP_DigestUpdate(EVP_MD_CTX *ctx, const void *d, size_t cnt);
 int EVP_DgiestFinal(EVP_MD_CTX *ctx, unsigned char *md, unsigned int *s);
 ```
 
 All the above functions returns 1 on success and 0 on failure.
 
 The `EVP_DigestInit` initialises the digest with the given `const EVP_MD` pointer. The `ctx` is of type `EVP_MD_CTX` and can be initialised as
 
 ```cpp
 EVP_MD_CTX *ctx = EVP_MD_CTX_create();
 ```
 
 and can be destroyed as,
 
 ```cpp
 EVP_MD_CTX_destroy(ctx);
 ```
 
 The Digest function can be initialised as,
 
 ```cpp
    int ret;
    EVP_MD_CTX *ctx;
 
    ctx = EVP_MD_CTX_create();
    if (!ctx) {
        return -1;
    }
    
    // take an example of the SHA256
    const EVP_MD *md = EVP_sha256();
    
    // init digest function
    ret = EVP_DigestInit(ctx, md);
    if (ret != 1) {
        return -1;
    }
    
    // digest function is initialised
```
 
 The `EVP_DigestUpdate` can be called more than once if there is more data to be hashed in. For a simple buffer, the `EVP_DigestUpdate` is called once.
 
 ```cpp
    ret = EVP_DigestUpdate(ctx, msg, msg_len);
    if (ret != 1) {
        return -1;
    }
    
    // input message msg has been update
    
 ```
 
 now the Final digest can be generated with a call to `EVP_DigestFinal` or `EVP_DigestFinal_ex` (Both does the same job).
 
 ```cpp
    unsigned int digest_len;
    uint8_t digest[64]; // 64 is the max digest length.. for now
    
    ret = EVP_DigestFinal_ex(ctx, digest, &digest_len);
    if (ret != 1) {
        return -1;
    }
    
    // final digest is in digest and the length is in digest_len
    
 ```
 
 The class definition then will become,
 
 ```cpp
 class hash_class {
    public:
        int generate_md5(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha1(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha224(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha256(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha384(uint8_t *msg, size_t msglen, uint8_t *digest);
        int generate_sha512(uint8_t *msg, size_t msglen, uint8_t *digest);
        
    private:
        int gen_hash(uint8_t *msg, size_t msglen, const EVP_MD *md, uint8_t *digest);
 };
 
 ```
 
 Below is the implementation of the class. The test code is a simple main function calling the class with sha256 algorithm.
 
 <script src="https://gist.github.com/DevNaga/5a79e166210f40af25f66c97a7121608.js"></script>
 
 
 
 
    
    
