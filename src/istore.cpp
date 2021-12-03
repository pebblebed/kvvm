#include "istore.hpp"

using namespace std;

Blob InMemoryStore::get(Hash k) const {
  lock_guard<std::mutex> _(mutex);
  auto ret = blobs.find(k);
  if (ret == blobs.end()) {
    throw invalid_argument("hash");
  }
  return ret->second;
}

void InMemoryStore::save(const Blob& b) {
  lock_guard<std::mutex> _(mutex);
  blobs.insert(make_pair(b.hash, b));
}
