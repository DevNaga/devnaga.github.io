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


<script src="https://gist.github.com/DevNaga/12fd047d41ce16d4a64bc576f0f1e0ef.js"></script>