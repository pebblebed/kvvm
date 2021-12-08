#include <iostream>

#include <gtest/gtest.h>

#include "../src/serialize.hpp"
#include "../src/istore.hpp"
#include "../src/table.hpp"

TEST(TableTest, testConstruction) {
    InMemoryStore store;
    Table t(store, Schema{ {} });

    auto ser = t.serialize();
    EXPECT_EQ(ser.size, MAGIC_BYTES + sizeof(size_t) + sizeof(Hash));
    EXPECT_EQ(*(uint64_t*)(&ser.bytes[MAGIC_BYTES]), 1);
}

TEST(TableTest, testAddCols) {
  InMemoryStore store;
  Table empty(store, Schema{ {} });

  empty.addCol("birthday", Cell::f(123456789.0));
  empty.addCol("name", Cell::s(""));
}
