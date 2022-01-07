#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "hps/src/hps.h"
#include "hash.hpp"
#include "istore.hpp"
#include "blob.hpp"

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
        return Blob(hps::to_string(flatten()));
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

