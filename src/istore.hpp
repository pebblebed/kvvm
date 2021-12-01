#pragma once

#include <mutex>
#include "hash.hpp"

template<typename Content>
class IStore {
protected:
    Content& current;

public:
    Hash getCurrentHash() {
        return current.hash();
    }

    // Returns getCurrentHash()
    virtual Hash cas(Hash oldval, Content& newval) = 0;
};

template<typename Content>
class InMemoryStore: protected IStore<Content> {
    std::mutex mutex;
    public:
    virtual bool cas(Hash oldval, Content& newval) {
        const std::lock_guard<std::mutex> _(mutex);
        auto curHash = IStore<Content>::current.hash();
        if (curHash == oldval) {
            auto newHash = newval.hash();
            if (newval == curHash) {
                return true;
            }
            IStore<Content>::current = newval;
            return true;
        }
        return false;
    }
};
