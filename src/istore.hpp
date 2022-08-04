#ifndef ISTORE_incl_
#define ISTORE_incl_

#include <mutex>
#include <unordered_map>

#include "hash.hpp"
#include "blob.hpp"

class IStore {
protected:
    Hash current;

public:
    Hash getCurrentHash() {
        return current;
    }

    // Returns getCurrentHash()
    virtual bool cas(Hash oldval, Hash newval) = 0;

    virtual Blob get(Hash h) const = 0;
    virtual void save(const Blob& b) = 0;

    // The "cache" is a memoization shortcut. Unlike "saved"
    // values, cached values might not hit on retrieval
    virtual void cacheSet(Hash hint, Hash target) {
        // just forget it by default
    }

    virtual std::pair<bool, Hash> cacheGet(Hash hint) {
        return std::make_pair(false, Hash());
    }
};

class InMemoryStore: public IStore {
    mutable std::mutex mutex;
    std::unordered_map<Hash, Blob, hash_hash> blobs;

    public:
    virtual bool cas(Hash oldval, Hash newval) {
        const std::lock_guard<std::mutex> _(mutex);
        auto curHash = current;
        if (curHash == oldval) {
            current = newval;
            return true;
        }
        return false;
    }

    virtual Blob get(Hash h) const;
    virtual void save(const Blob& b);
};

#endif
