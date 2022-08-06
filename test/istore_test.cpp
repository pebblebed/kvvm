#include <gtest/gtest.h>
#include "../src/istore.hpp"

TEST(IStoreTest, Root) {
    InMemoryRoot root;
    auto val = root.getCurrentHash();
    uint8_t bytes[] = { 0xa, 0xbb, 0xcc };
    auto myHash = computeHash(bytes, 3);
    auto success = root.cas(val, myHash);
    EXPECT_TRUE(success);

    success = root.cas(val, myHash);
    EXPECT_FALSE(success);
    auto newHash = root.getCurrentHash();
    EXPECT_EQ(newHash, myHash);
}

TEST(IStoreTest, Store) {
    uint8_t bytes[] = { 0xa, 0xbb, 0xcc };
    Blob b(bytes, 3);
    InMemoryStore store;

    store.save(b);
    auto b2 = store.get(b.hash);
    EXPECT_EQ(b2.hash, b.hash);
    EXPECT_EQ(b2, b);
}

TEST(IStoreTest, Cache) {
    auto h = computeHash(nullptr, 0);
    InMemoryCache c;
    std::string name("hiya");

    EXPECT_EQ(c.getStats().misses, 0);
    EXPECT_EQ(c.getStats().hits, 0);

    Hash hOut;
    auto succ = c.lookup(name, hOut);
    EXPECT_FALSE(succ);
    EXPECT_EQ(c.getStats().misses, 1);

    c.record(name, h);
    succ = c.lookup(name, hOut);
    EXPECT_TRUE(succ);
    EXPECT_EQ(hOut, h);
    EXPECT_EQ(c.getStats().misses, 1);
    EXPECT_EQ(c.getStats().hits, 1);
}
