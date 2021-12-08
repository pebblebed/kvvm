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
    hps::to_string<int>((int)id, ret);
    hps::to_string<size_t>(hashes.size(), ret);
    for (auto &h: hashes) {
        hps::to_string<std::array<uint8_t, HASH_BYTES>>(h.bytes, ret);
    }
    return Blob(ret);
}

HashedStructResult deserializeHashStructure(const Blob& b) {
  HashedStructResult ret;
  string toParse((const char*)b.bytes, b.size);
  int id;
  hps::from_string<int>(toParse, id);
  if (magicToStrMap.find((Magic)id) == magicToStrMap.end()) {
    throw std::invalid_argument("invalid magic field");
  }
  size_t len;
  hps::from_string<size_t>(toParse, len);
  ret.hashen.reserve(len);
  for (size_t i = 0; i < len; i++) {
    array<uint8_t, HASH_BYTES> a;
    hps::from_string<std::array<uint8_t, HASH_BYTES>>(toParse, a);
    ret.hashen.push_back(Hash(a));
  }
  return ret;
}
