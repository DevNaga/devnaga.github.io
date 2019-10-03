---
layout: post
title:  "Deriving keys with HKDF"
date:   2019-10-01 23:26:00
categories: technology, cryptography, keyexchange, mbedtls
---


HKDF is another KDF mechanism that allows a device to derive a key. Not one key, but one to 255 keys.

HKDF has two stages.

1. Extract
2. Expand

The inputs for the HKDF are input keying material or IKM, a salt and a info field. The Extract phase extracts a PRK out of the input key material and the expand function expands the PRK from 1 to 255 keys. The key length is depend on the used MD.

Often, the uses of this comes when the system uses an ECDH to derive a shared secret, it must be fed through the KDFs to derive a strong random key.

Assuming the IKM is a shared secret that is already present between the sender and receiver or a server and client. A salt is shared between the sender and receiver (someone derving it from a PRNG device and sharing it with other)

The info field is usually an optional field, however, it can be used to exploit or assign keys to a particular device thus serve as a distinguish factor.

Lets' look at the Mbedtls HKDF API.

```c

int mbedtls_hkdf( const mbedtls_md_info_t *md, const unsigned char *salt,
                  size_t salt_len, const unsigned char *ikm, size_t ikm_len,
                  const unsigned char *info, size_t info_len,
                  unsigned char *okm, size_t okm_len );

```

this is defined in `/usr/include/mbedtls/hkdf.h`.

the `md` field can be derived as

```c

const mbedtls_md_info_t *md = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

```

we chose SHA256 as our MD in this case.

The remaining fields are already discussed. The `okm` consists the set of output derived keys and the `okm_len` must be 255 * 32. 32 is the SHA256 hash value size.

With all this, below is a sample example of the key derivation with the HKDF. Compile with `-lmbedcrypto`.

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <mbedtls/hkdf.h>
#include <stdint.h>
#include <string.h>

// salt is shared between server and client
uint8_t salt[32];

void server()
{
    // input key .. must be setup during one time provisioning or OTP
    uint8_t input[] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
    };

    // info can be a device identifier / some stored ids within device  from EEPROM etc
    uint8_t info[] = {
        0xde, 0xad, 0xbe, 0xef
    };

    // generate a salt
    memset(salt, 0, sizeof(salt));
    {
        int fd;

        fd = open("/dev/urandom", O_RDONLY);
        if (fd < 0) {
            printf("failed to generate a random\n");
            return;
        }

        read(fd, salt, sizeof(salt));
        close(fd);
    }

    // below series of steps derive the key

    uint8_t okm[32];

    memset(okm, 0, sizeof(okm));

    // choose SHA256 for MD
    const mbedtls_md_info_t *mdinfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (!mdinfo) {
        return;
    }

    int ret;

    ret = mbedtls_hkdf(mdinfo, salt, sizeof(salt), input, sizeof(input), info, sizeof(info), okm, sizeof(okm));
    if (ret != 0) {
        printf("failed to hkdf\n");
        return;
    }

    printf("HKDF derived at server: ");

    int i;

    for (i = 0; i < sizeof(okm); i ++) {
        printf("%02x ", okm[i]);
    }
    printf("\n");
}

void client()
{

    // input key .. must be setup during manufacturing
    uint8_t input[] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
    };


    // info can be a device identifier / some stored ids within device  from EEPROM etc
    uint8_t info[] = {
        0xde, 0xad, 0xbe, 0xef
    };

    uint8_t okm[32];

    memset(okm, 0, sizeof(okm));

    // choose SHA256 as MD
    const mbedtls_md_info_t *mdinfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if (!mdinfo) {
        return;
    }

    int ret;

    ret = mbedtls_hkdf(mdinfo, salt, sizeof(salt), input, sizeof(input), info, sizeof(info), okm, sizeof(okm));
    if (ret != 0) {
        printf("failed to hkdf\n");
        return;
    }

    printf("HKDF derived at client: ");

    int i;

    for (i = 0; i < sizeof(okm); i ++) {
        printf("%02x ", okm[i]);
    }
    printf("\n");
}

int main()
{
    // server creates a salt and shares it with client
    //
    // server assumes the input key material is commonly known and setup
    // between server and client within their hardware / read only memory
    //
    // salt is shared
    //
    // device identifier is used as info field for a device specific key generation
    server();
    client();
}

```

