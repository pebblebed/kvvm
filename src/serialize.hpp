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
 * OutBuf types have a "produce(uint8_t)" method.
 * InBuf has a "consume(uint8_t)".
 */
// varint impl
template<typename T> int clz(T x);

template<typename I, typename Out> void encode(I i, Out out) {
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

template<typename I, typename In> I decode(In in) {
    auto nBytes = in.consume();
    I retval = 0;
    if (nBytes == 0) return retval;
    for (auto ii = 0; ii < nBytes; ii++) {
        retval = (retval << 8) + in.consume();
    }
	return retval;
}

class OutBuffer {
    std::string& buf;
public:
    OutBuffer(std::string& buf) : buf(buf) {} 
    void produce(uint8_t b) {
        buf.resize(buf.size() + 1);
        buf[buf.size() - 1] = b;
    }
};

class InBuffer {
    const std::string& buf;
    std::string::const_iterator it;
public:
    InBuffer(const std::string& buf) : buf(buf), it(buf.begin()) { }

    uint8_t consume() {
        return uint8_t(*it++);
    }
};

static inline std::string to_string(const int i) {
    std::string retval;
    OutBuffer out(retval);
    encode(i, out);
    return retval;
}

std::string to_string(long unsigned int li);
std::string to_string(const HashedStruct& hs);

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

    template<typename B> void
    parse(B& b) {
        b >> (int&)magic >> hashen;
    }

    template<typename B> void
    serialize(B& b) const {
        b << (int)magic << hashen;
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
        return Blob(SerImpl::to_string(flatten()));
    }

    template<typename B>
    void serialize(B& out) const {
        out << flatten();
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

