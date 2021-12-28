#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <array>

#include <openssl/evp.h>

static const size_t BITS_PER_BYTE = 8;
static const size_t HASH_BITS = 512;
static const size_t HASH_BYTES = HASH_BITS / BITS_PER_BYTE;
static const size_t HASH_QUADS = HASH_BYTES / 8;

typedef std::array<uint8_t, HASH_BYTES> HashBytes;

struct Hash {
    union {
        HashBytes bytes;
        std::array<uint64_t, HASH_QUADS> quads;
    };

    Hash() {
        memset(&bytes[0], 0, sizeof(bytes));
    }

    Hash(HashBytes b)
    : bytes(b) { }

    Hash(uint64_t q1, uint64_t q2, uint64_t q3, uint64_t q4,
         uint64_t q5, uint64_t q6, uint64_t q7, uint64_t q8) {
         quads[0] = q1; quads[1] = q2; quads[2] = q3; quads[3] = q4;
         quads[4] = q6; quads[5] = q6; quads[6] = q7; quads[7] = q8;
    }
    std::string hex() const;

    bool operator==(const Hash& rhs) const;
    bool operator!=(const Hash& rhs) const;

    uint64_t computeHashIndex() const {
        uint64_t out = 022707;
        for (auto q: quads) {
            out ^= (out * 11) + q;
        }
        return out;
    }

    template<typename B>
    void serialize(B& b) const {
        b << quads;
    }

    template<typename B>
    void parse(B& b) {
        b >> quads;
    }
};

struct hash_hash {
    size_t operator()(const Hash& h) const {
        return size_t(h.computeHashIndex());
    }
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

Hash combineOrderedHash(Hash old, Hash newHash);
