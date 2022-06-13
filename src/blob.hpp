#ifndef BLOB_incl_
#define BLOB_incl_
#include <atomic>
#include <cstdlib>
#include <cstdint>
#include <stdlib.h>
#include <assert.h>

#include "dbg.hpp"
#include "hash.hpp"

/*
 * Your humble, content-addressable, immutable Blob. We decide to store
 * memory out of line with size/content, and the blob owns a copy of the
 * memory in question. This increases copying, alas, but is simpler to
 * reason about for an initial implementation.
 */
struct Blob {
    const Hash hash;
    const size_t size;
    const uint8_t *bytes;

    Blob(const uint8_t* bts, size_t size);
    Blob(std::string s);
    // Need a copy ctor since Blob() has copy semantics
    Blob(const Blob& rhs);
    ~Blob();
    bool check() const;

    std::string substr(int start, size_t len) const;
    std::string string() const;
};

#endif