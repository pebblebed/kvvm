#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <vector>
#include <cstdio>


#include "serialize.hpp"
#include "hps.h"

using namespace std;

Blob serializeHashStructure(Magic id, vector<Hash> hashes) {
    string ret;
    hps::to_string(id, ret);
    hps::to_string(hashes.size(), ret);
    for (auto h: hashes) {
      hps::to_string(h.bytes, ret);
    }
    return Blob(ret);
}

HashedStructResult deserializeHashStructure(const Blob& b) {
  HashedStructResult ret;
  if (b.size < MAGIC_BYTES + sizeof(size_t)) {
    throw std::invalid_argument("undersized hash structure blob");
  }
  auto magicStr = std::string((const char*)b.bytes, MAGIC_BYTES);
  auto magicp = magicToStrMap.find(magicStr);

  if (magicp = magicToStrMap.end()) {
    throw std::invalid_argument(std::string("unknown structure ID: ") +
      magicStr);
  }
  ret.magic = *magicp;
  int idx = MAGIC_BYTES;

  uint64_t nhashes;
  memcpy((char*)&nhashes, &b.bytes[idx], sizeof(uint64_t));
  nhashes = ntohll(nhashes);
  idx += sizeof(uint64_t);

  if (b.size != (MAGIC_BYTES + sizeof(size_t) + nhashes * HASH_BYTES)) {
    throw std::invalid_argument("wrong blob length");
  }
  ret.hashen.reserve(nhashes);
  for (size_t i = 0; i < nhashes; i++) {
    Hash h;
    memcpy(h.bytes, b.bytes + idx, HASH_BYTES);
    ret.hashen.push_back(h);
    idx += HASH_BYTES;
  }
}
