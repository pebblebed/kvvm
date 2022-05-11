#pragma once

#include <unordered_map>
#include <string>
#include <vector>

// #include "hps/src/hps.h"
#include "hash.hpp"
#include "istore.hpp"
#include "blob.hpp"

struct HashedStruct;

namespace SerImpl {

/*
 * Simple bytebuffer classes. Creator of the buffer owns the storage; i.e.,
 * underlying std::string is not managed by these classes.
 */

class InBuffer {
    const std::string& buf;
    std::string::const_iterator it;
public:
    InBuffer(const std::string& buf) : buf(buf), it(buf.begin()) { }
    uint8_t consume();
};

class OutBuffer {
    std::string& buf;
public:
    OutBuffer(std::string& buf) : buf(buf) {}
    void produce(uint8_t b);
    const std::string& str() const { return buf; }
    InBuffer asInBuffer() const { return InBuffer(buf); }
    Blob toBlob() const;
};

// varint impl
template<typename T> int clz(T x);

template<typename I> void encode_int_impl(I i, OutBuffer& out) {
    if (i == 0) { out.produce(0); return; }

    auto maxBits = sizeof(I) * 8;
    int leadingZeros = clz((I)i);
    assert(leadingZeros < maxBits);
    auto nonZeroBytes = sizeof(I) - leadingZeros / 8;
    assert(nonZeroBytes > 0);
    assert(nonZeroBytes < UINT8_MAX);
    out.produce(nonZeroBytes);
    for (auto ii = 0; ii < nonZeroBytes; ii++) {
        out.produce(i & 0xff);
        i >>= 8;
	}
}

void encode(std::string s, OutBuffer& b);
void encode(uint64_t u64, OutBuffer& b);
void encode(uint8_t u8, OutBuffer& b);

template<typename Inner>
void encode_vector(const std::vector<Inner>& vec, OutBuffer& b) {
    encode(vec.size(), b);
    for (const auto& inner: vec) {
        encode(inner, b);
    }
}

template<typename Inner>
void encode(const std::vector<Inner>& val, OutBuffer& b) {
    encode_vector<Inner>(val, b);
}

void decode(InBuffer& in, std::string& str);
void decode(InBuffer& in, uint64_t u64);

template<typename Inner>
void decode_vector(std::vector<Inner>& vec, InBuffer& b) {
    size_t sz;
    decode(b, sz);
    vec.reserve(sz);
    for (typeof(sz) i = 0; i < sz; i++) {
        Inner inner;
        vec.emplace_back(decode(b, inner));
    }
}

}

/*
 * A lot of structures are merkle graphs. We're tackling a DBMS first
 * because those graphs are guaranteed to be merkle _trees_ by
 * construction, making many things simpler.
 *
 * We represent these as magic/list-of-hash paylaods, with node-specific
 * ideas about how to serde their runtime representation into a list of
 * hashes.
 */
const int MAGIC_BYTES = 6;

#define MAGICS() \
   MAGIC(SCHEMA) \
   MAGIC(ROWBNK) \
   MAGIC(_TABLE)

enum Magic {
#define MAGIC(x) \
    MAGIC_ ##x,

    MAGICS()
#undef MAGIC
};

static const std::unordered_map<Magic, const char*> magicToStrMap {
#define MAGIC(x) \
    { MAGIC_ ##x, #x },
    MAGICS()
#undef MAGIC
};

static const std::unordered_map<std::string, Magic> strToMagicMap {
#define MAGIC(x) \
    { std::string(#x), MAGIC_ ##x },
    MAGICS()
#undef MAGIC
};
#undef MAGICS

struct HashedStruct {
    Magic magic;
    std::vector<Hash> hashen;

    static HashedStruct
    decode(SerImpl::InBuffer& buf) {
        assert(false); // not implemented
    }

    void
    encode(SerImpl::OutBuffer& buf) const {
        assert(false); // not implemented
    }
};

struct BlobNode {
    virtual Blob toBlob() const = 0;

    Hash hash() const {
        return toBlob().hash;
    }

    void christen(IStore& istore) const {
        istore.save(toBlob());
    }
};

/*
 * Internal nodes are always uniquely representable as a tag/magic number, and
 * vector of hashes, aka a HashedStruct.
 */
struct BlobInternalNode : public BlobNode {
    virtual HashedStruct flatten() const = 0;

    Blob toBlob() const {
        std::string data;
        SerImpl::OutBuffer outbuf(data);
        flatten().encode(outbuf);
        return Blob(outbuf.str());
    }
};

template<typename Underlying /* as Serializable */>
struct Hashable {
    Hash hash;

    Hashable(const Hash h)
        : hash(h)
    { }

    Hashable(const Underlying& u)
        : hash(u.hash())
    { }

    Underlying materialize(IStore& store) {
        return Underlying::deserialize(store.get(hash));
    }
};

