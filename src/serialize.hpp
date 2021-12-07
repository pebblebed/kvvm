#pragma once

#include <string>
#include <unordered_map>

#include "hash.hpp"

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
#undef MAGICS

std::string serializeHashStructure(Magic id, std::vector<Hash> hashes);
