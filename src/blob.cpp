#include "blob.hpp"

Blob::Blob(const uint8_t* bts, size_t size)
    : hash(computeHash(bts, size))
    , size(size)
{
    auto vbytes = std::malloc(size);
    dbg(blob, 3, "blob alloc (%zd) -> %p\n", size, vbytes);
    if (vbytes == nullptr) {
        throw std::bad_alloc();
    }
    memcpy(vbytes, bts, size);
    bytes = (uint8_t*)vbytes;
}

Blob::Blob(std::string s)
: Blob((const uint8_t*)&s[0], s.length()) { }

Blob::Blob(const Blob& rhs)
: Blob(rhs.bytes, rhs.size) { }

Blob::~Blob() {
    dbg(blob, 3, "blob free (%p) sz %zd\n", bytes, size);
    std::free((void*)bytes);
}

bool
Blob::check() const {
    HashState hs;
    return hs(bytes, size) == hash;
}

std::string
Blob::substr(int start, size_t len) const {
    assert(start + len <= size);
    return std::string((const char*)bytes + start, len);
}

std::string
Blob::string() const {
        return substr(0, size);
}