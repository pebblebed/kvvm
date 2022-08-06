#ifndef ISTORE_incl_
#define ISTORE_incl_

#include <mutex>
#include <unordered_map>

#include "hash.hpp"
#include "blob.hpp"

struct IRoot {
    virtual Hash getCurrentHash() const = 0;
    virtual bool cas(Hash oldval, Hash newval) = 0;
};

struct IStore {
    virtual Blob get(Hash h) const = 0;
    virtual void save(const Blob& b) = 0;
};

struct ICache {
    struct Stats {
        int inserts, hits, misses;
    };

    virtual void record(std::string name, Hash hash) = 0;
    virtual bool lookup(std::string name, Hash& out) = 0;

    virtual Stats getStats() const = 0;

};

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

class InMemoryStore: public IStore {
    mutable std::mutex mutex;
    std::unordered_map<Hash, Blob, hash_hash> blobs;

    public:
    virtual Blob get(Hash h) const;
    virtual void save(const Blob& b);
};

// The Cache is a fast-forward memoization store that remembers shortcuts
// to blobs. Since it's intended only for referentially transparent use,
// it does not support overwrites or deletes.
class InMemoryCache : public ICache {
    std::unordered_map<std::string, Hash> cache_;
    int maxItems_;
    Stats stats_;

    public:
    InMemoryCache(int maxItems = 32)
    : maxItems_(maxItems)
    , stats_({})
    { }

    virtual void record(std::string name, Hash hash);
    virtual bool lookup(std::string name, Hash& out);

    virtual Stats getStats() const { return stats_; }

    protected:
    void replace();
};


#endif
