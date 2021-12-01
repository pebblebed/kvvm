#pragma once

#include <atomic>
#include <cstdlib>
#include <cstdint>
#include <stdlib.h>

#include "hash.hpp"

/*
 * Your humble, content-addressable, immutable Blob. We decide to store
 * memory out of line with size/content, and the blob owns a copy of the
 * memory in question. This increases copying, alas, but is simpler to
 * reason about for an initial implementation.
 */
struct Blob {
    const Hash hash;
    const uint64_t hashIndex;
    const size_t size;
    const uint8_t *bytes;

    Blob(const uint8_t* bts, size_t size)
        : hash(computeHash(bts, size))
        , hashIndex(computeHashIndex(hash))
        , size(size)
        {
        auto vbytes = std::malloc(size);
        if (vbytes == nullptr) {
            throw std::bad_alloc();
        }
        memcpy(vbytes, bts, size);
        bytes = (uint8_t*)vbytes;
    }

    ~Blob() {
        std::free((void*)bytes);
    }

    bool check() const {
        HashState hs;
        return hs(bytes, size) == hash;
    }

    private:
    static uint64_t computeHashIndex(Hash hash) {
        uint64_t out = 022707;
        for (auto q: hash.quads) {
            out *= 37;
            out += q;
        }
        return out;
    }
};

