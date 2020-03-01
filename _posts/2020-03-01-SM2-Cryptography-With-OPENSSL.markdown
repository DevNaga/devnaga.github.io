---
layout: post
title:  "SM2 Cryptography with OpenSSL (Sign and Verify)"
date:   2020-03-01 23:26:00
categories: technology,cryptography, c, yocto, android
---

SM2 is a chinese [specification](https://tools.ietf.org/html/draft-shen-sm2-ecdsa-02) and has support for openssl.

So A sign and verify can be performed. Usecases : mostly useful in any authentication software used in chinese devices etc.

select `NID_sm2` when creating a key : `EC_KEY_new_by_curve_name` and everything in Sign and Verify is same.

Below is an example of SM2 class with openssl.

```cpp
class sm2_crypto {
    public:
        explicit sm2_crypto() {
            OpenSSL_add_all_algorithms();
        }
        ~sm2_crypto() {
        }

        int generate_key(std::string pubkeyfile, std::string privkeyfile);
        int sign(const uint8_t *text_msg, size_t text_msg_len, uint8_t *signature, size_t *signature_len);
        int verify(const uint8_t *text_msg, int text_msg_len, uint8_t *signature, size_t signature_len);
};
```

Below is the full example

<script src="https://gist.github.com/DevNaga/ba702d1b1dc42d0639b090b5ad6c94a8.js"></script>


