#pragma once

#include <unordered_map>

#include "hash.hpp"
#include "blob.hpp"

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

Blob serializeHashStructure(Magic id, std::vector<Hash> hashes);

struct HashedStructResult {
    Magic magic;
    std::vector<Hash> hashen;
};
HashedStructResult deserializeHashStructure(const Blob& b);
