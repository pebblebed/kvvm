#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <vector>
#include <cstdio>
#include <iostream>
#include <strstream>

#include "serialize.hpp"
#include "hps.h"

using namespace std;

Blob serializeHashStructure(Magic id, vector<Hash> hashes) {
    ostrstream out;
    hps::to_stream<int>((int)id, out);
    hps::to_stream<size_t>(hashes.size(), out);
    for (auto &h: hashes) {
        hps::to_stream<std::array<uint8_t, HASH_BYTES>>(h.bytes, out);
    }
    return Blob(out.str());
}

HashedStructResult deserializeHashStructure(const Blob& b) {
  HashedStructResult ret;
  istringstream in(b.string());
  int id;
  hps::from_stream<int>(in, id);
  if (magicToStrMap.find((Magic)id) == magicToStrMap.end()) {
    throw std::invalid_argument("invalid magic field");
  }
  size_t len;
  hps::from_stream<size_t>(in, len);
  ret.hashen.reserve(len);
  for (size_t i = 0; i < len; i++) {
    array<uint8_t, HASH_BYTES> a;
    hps::from_stream<std::array<uint8_t, HASH_BYTES>>(in, a);
    ret.hashen.push_back(Hash(a));
  }
  return ret;
}
