#include "istore.hpp"

using namespace std;

bool
InMemoryRoot::cas(Hash oldval, Hash newval) {
    const std::lock_guard<std::mutex> _(mutex);
    auto curHash = current;
    if (curHash == oldval) {
        current = newval;
        return true;
    }
    return false;
}

Blob
InMemoryStore::get(Hash k) const {
  lock_guard<std::mutex> _(mutex);
  auto ret = blobs.find(k);
  if (ret == blobs.end()) {
    throw invalid_argument("hash");
  }
  return ret->second;
}

void
InMemoryStore::save(const Blob& b) {
  lock_guard<std::mutex> _(mutex);
  blobs.insert(make_pair(b.hash, b));
}

void
InMemoryCache::record(std::string name, Hash hash) {
    lock_guard<std::mutex> _(mutex);
    auto it = cache_.find(name);

    if (it != cache_.end()) {
        assert(it->second == hash);
        return;
    }

    stats_.inserts++;
    cache_[name] = hash;
}

bool
InMemoryCache::lookup(std::string name, Hash& out) {
    lock_guard<std::mutex> _(mutex);
    auto it = cache_.find(name);

    if (it != cache_.end()) {
        stats_.hits++;
        out = it->second;
        return true;
    }
    stats_.misses++;
    return false;
}
