#include <vector>
#include <assert.h>
#include <stdio.h>
#include <cstdio>

#include "serialize.hpp"

using namespace std;

string serializeHashStructure(Magic id, vector<Hash> hashes) {
    string ret;
    ret.resize(MAGIC_BYTES + sizeof(size_t) + HASH_BYTES * hashes.size());
    auto magI = magicToStrMap.find(id);
    assert(magI != magicToStrMap.end());
    fprintf(stderr, "found it! %s\n", magI->second);
    auto magStr = magI->second;
    fprintf(stderr, "stupendous %lx =? %lx\n", long(strlen(magStr)), long(MAGIC_BYTES));
    assert(strlen(magStr) == MAGIC_BYTES);

    int idx = 0;
    memcpy(&ret[idx], &magStr[0], MAGIC_BYTES); idx += MAGIC_BYTES;
    auto sz = hashes.size();
    memcpy(&ret[idx], (const char*)&sz, sizeof(sz)); idx += sizeof(sz);

    for (auto h: hashes) {
      memcpy(&ret[idx], h.bytes, HASH_BYTES); idx += HASH_BYTES;
    }
    return ret;
}

