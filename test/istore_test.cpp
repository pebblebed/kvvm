#include <gtest/gtest.h>
#include "../src/istore.hpp"

TEST(IStoreTest, TestRootCas) {
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

TEST(IStoreTest, TestRecall) {
    uint8_t bytes[] = { 0xa, 0xbb, 0xcc };
    Blob b(bytes, 3);
    InMemoryStore store;

    store.save(b);
    auto b2 = store.get(b.hash);
    EXPECT_EQ(b2.hash, b.hash);
    EXPECT_EQ(b2, b);
}
