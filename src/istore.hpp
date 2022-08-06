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
        int inserts, evictions;
        int hits, misses;
    };

    virtual void record(std::string name, Hash hash) = 0;
    virtual bool lookup(std::string name, Hash& out) const = 0;

    virtual Stats getStats() const = 0;
};

struct IData {
    virtual IRoot& root() = 0;
    virtual IStore& store() = 0;
    virtual ICache& cache() = 0;

    virtual const IRoot& root() const = 0;
    virtual const IStore& store() const = 0;
    virtual const ICache& cache() const = 0;

    // Root methods
    Hash getCurrentHash() const {
        return root().getCurrentHash();
    }
    bool cas(Hash oldval, Hash newval) {
        return root().cas(oldval, newval);
    }

    // Store methods
    void save(Blob b) { store().save(b); }
    Blob get(Hash h) const {
        return store().get(h);
    }

    // Cache methods
    void record(std::string name, Hash hash) {
        cache().record(name, hash);
    }

    bool lookup(std::string name, Hash& hash) const {
        return cache().lookup(name, hash);
    }

    ICache::Stats getStats() const {
        return cache().getStats();
    }
};

extern IData* getTestData();

#endif
