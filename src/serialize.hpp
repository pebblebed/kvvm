#ifndef SERIALIZE_incl_
#define SERIALIZE_incl_

#include <unordered_map>
#include <string>
#include <vector>

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

void encode(std::string s, OutBuffer& b);
void encode(uint64_t u64, OutBuffer& b);
void encode(int64_t i64, OutBuffer& b);
void encode(uint8_t u8, OutBuffer& b);
void encode(double d, OutBuffer& b);
void encode(float f, OutBuffer& b);
void encode(bool boo, OutBuffer& b);
void encode(Hash h, OutBuffer& b);

template<typename VInner>
void encode(const VInner& vec, OutBuffer& b) {
    encode(vec.size(), b);
    for (const auto& inner: vec) {
        encode(inner, b);
    }
}

/* --- */
void decode(InBuffer& in, std::string& str);
void decode(InBuffer& in, uint64_t& u64);
void decode(InBuffer& in, int64_t& i64);
void decode(InBuffer& in, double& d);
void decode(InBuffer& in, bool& b);
void decode(InBuffer& in, uint8_t& b);
void decode(InBuffer& in, Hash& h);

template<typename VInner>
void decode(InBuffer& b, VInner& vec) {
    size_t sz;
    decode(b, sz);
    vec.reserve(sz);
    for (size_t i = 0; i < sz; i++) {
        typename VInner::value_type inner;
        decode(b, inner);
        vec.push_back(inner);
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

#define MAGICS() \
    MAGIC(SCHEMA) \
    MAGIC(ROWBNK) \
    MAGIC(_TABLE) \
    MAGIC(_SLICE) \
    MAGIC(FILTER) \
    MAGIC(__JOIN)

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
    decode(SerImpl::InBuffer& ) {
        assert(false); // not implemented
    }

    void
    encode(SerImpl::OutBuffer& ) const {
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


#endif
