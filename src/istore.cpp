#include <stdexcept>
#include "istore.hpp"

using namespace std;

class InMemoryRoot: public IRoot {
protected:
    mutable std::mutex mutex;
    Hash current;

public:
    Hash getCurrentHash() const {
        return current;
    }

    virtual bool cas(Hash oldVal, Hash newVal);
};

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

class InMemoryStore: public IStore {
    mutable std::mutex mutex;
    std::unordered_map<Hash, Blob, hash_hash> blobs;

    public:
    virtual Blob get(Hash h) const;
    virtual void save(const Blob& b);
};

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

// The Cache is a fast-forward memoization store that remembers shortcuts
// to blobs. Since it's intended only for referentially transparent use,
// it does not support overwrites or deletes.
class InMemoryCache : public ICache {
    std::unordered_map<std::string, Hash> cache_;
    int maxItems_;
    mutable Stats stats_;

    public:
    InMemoryCache(int maxItems = 32)
    : maxItems_(maxItems)
    , stats_({})
    { }

    virtual void record(std::string name, Hash hash);
    virtual bool lookup(std::string name, Hash& out) const;

    virtual Stats getStats() const { return stats_; }

    protected:
    void replace();
};

void
InMemoryCache::record(std::string name, Hash hash) {
    lock_guard<std::mutex> _(mutex);
    auto it = cache_.find(name);

    if (it != cache_.end()) {
        assert(it->second == hash);
        return;
    }

    if (cache_.size() >= maxItems_) {
        stats_.evictions++;
        // XXX: finish me.
        auto it = cache_.begin();
        cache_.erase(it);
    }
    stats_.inserts++;
    cache_[name] = hash;
}

bool
InMemoryCache::lookup(std::string name, Hash& out) const {
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

class InMemoryData : public IData {
    InMemoryRoot root_;
    InMemoryStore store_;
    InMemoryCache cache_;

    virtual IRoot& root() { return root_; }
    virtual IStore& store() { return store_; }
    virtual ICache& cache() { return cache_; }

    virtual const IRoot& root() const { return root_; }
    virtual const ICache& cache() const { return cache_; }
    virtual const IStore& store() const { return store_; }
};

IData* getTestData() {
    return new InMemoryData();
}
