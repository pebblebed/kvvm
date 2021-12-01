#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <cstring>

static const size_t BITS_PER_BYTE = 8;
static const size_t HASH_BITS = 256;
static const size_t HASH_BYTES = HASH_BITS / BITS_PER_BYTE;

struct Hash {
    uint8_t bytes[HASH_BYTES];
    Hash() {
        memset(&bytes, 0, sizeof(bytes));
    }

	std::string hex() const;
};

class HashState {
    protected:
    Hash state;

    public:

    const Hash currentState() const {
        return state;
    }

	void operator()(const uint8_t* bytes, size_t sz);
};

