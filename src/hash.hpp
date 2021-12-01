#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <cstring>

#include <openssl/evp.h>

static const size_t BITS_PER_BYTE = 8;
static const size_t HASH_BITS = 256;
static const size_t HASH_BYTES = HASH_BITS / BITS_PER_BYTE;

struct Hash {
    union {
        uint8_t bytes[HASH_BYTES];
        uint64_t quads[HASH_BYTES / 8];
    };
    public:
    Hash() {
        memset(bytes, 0, sizeof(bytes));
    }
	std::string hex() const;

    bool operator==(const Hash& rhs) const;
    bool operator!=(const Hash& rhs) const;
};

class HashState {
    protected:
    const EVP_MD* algorithm;
    EVP_MD_CTX* context;

    public:
    HashState();
    ~HashState();

    // NB: Can only do this once per HashState currently. Yucko.
	Hash operator()(const uint8_t* bytes, size_t sz);
};

Hash computeHash(const uint8_t* bytes, size_t sz);
