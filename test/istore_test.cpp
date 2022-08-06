#include <gtest/gtest.h>
#include "../src/istore.hpp"

TEST(IStoreTest, Root) {
    auto d = getTestData();
    auto val = d->getCurrentHash();
    uint8_t bytes[] = { 0xa, 0xbb, 0xcc };
    auto myHash = computeHash(bytes, 3);
    auto success = d->cas(val, myHash);
    EXPECT_TRUE(success);

    success = d->cas(val, myHash);
    EXPECT_FALSE(success);
    auto newHash = d->getCurrentHash();
    EXPECT_EQ(newHash, myHash);
}

TEST(IStoreTest, Store) {
    uint8_t bytes[] = { 0xa, 0xbb, 0xcc };
    Blob b(bytes, 3);

    auto d = getTestData();
    d->save(b);
    auto b2 = d->get(b.hash);
    EXPECT_EQ(b2.hash, b.hash);
    EXPECT_EQ(b2, b);
}

TEST(IStoreTest, Cache) {
    auto h = computeHash(nullptr, 0);
    auto d = getTestData();
    std::string name("hiya");

    EXPECT_EQ(d->getStats().misses, 0);
    EXPECT_EQ(d->getStats().hits, 0);

    Hash hOut;
    auto succ = d->lookup(name, hOut);
    EXPECT_FALSE(succ);
    EXPECT_EQ(d->getStats().misses, 1);

    d->record(name, h);
    succ = d->lookup(name, hOut);
    EXPECT_TRUE(succ);
    EXPECT_EQ(hOut, h);
    EXPECT_EQ(d->getStats().misses, 1);
    EXPECT_EQ(d->getStats().hits, 1);
}
