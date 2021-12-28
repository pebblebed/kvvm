#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>

#include "serialize.hpp"
#include "hps.h"
#include "dbg.hpp"

using namespace std;

Blob serializeHashStructure(Magic id, vector<Hash> hashes) {
    ostringstream out;
    hps::to_stream<int>((int)id, out);
    dbg(ser, 0, "outbufsz: %zd\n", out.str().size());
    hps::to_stream<size_t>(hashes.size(), out);
    dbg(ser, 0, "outbufsz after hashes size: %zd\n", out.str().size());
    for (auto &h: hashes) {
        hps::to_stream<std::array<uint8_t, HASH_BYTES>>(h.bytes, out);
        dbg(ser, 0, "outbufsz after another has: %zd\n", out.str().size());
    }
    return Blob(out.str());
}

HashedStructResult deserializeHashStructure(const Blob& b) {
  HashedStructResult ret;
  istringstream in(b.string());
  dbg(ser, 0, "streampos: %zd\n", size_t(in.tellg()));
  int id = hps::from_stream<int>(in);
  if (in.bad()) {
    dbg(ser, 0, "streampos bad! BAD!!!\n");
    throw std::runtime_error("invalid buffer");
  }
  if (in.eof()) {
    dbg(ser, 0, "streampos too short!\n");
    throw std::runtime_error("invalid buffer");
  }
  dbg(ser, 0, "streampos after id: %zd\n", size_t(in.tellg()));
  if (magicToStrMap.find((Magic)id) == magicToStrMap.end()) {
    throw std::invalid_argument("invalid magic field");
  }
  ret.magic = (Magic)id;
  size_t len = hps::from_stream<size_t>(in);
  ret.hashen.reserve(len);
  for (size_t i = 0; i < len; i++) {
    array<uint8_t, HASH_BYTES> a;
    hps::from_stream<std::array<uint8_t, HASH_BYTES>>(in, a);
    ret.hashen.push_back(Hash(a));
  }
  return ret;
}
