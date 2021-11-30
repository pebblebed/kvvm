#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <cstring>

static const size_t HASH_LENGTH = 256;

struct HashResult {
    uint8_t bytes[HASH_LENGTH];
    HashResult() {
        memset(&bytes, 0, sizeof(bytes));
    }

	std::string hex();
};

class HashState {
    public:
    static const size_t HASH_LENGTH = 256;

    protected:
    HashResult state;

    public:

    const HashResult currentState() const {
        return state;
    }

	void operator()(const uint8_t* bytes, size_t sz);
};

