#include <iostream>

#include <gtest/gtest.h>

#include "../src/serialize.hpp"
#include "../src/istore.hpp"
#include "../src/table.hpp"

TEST(TableTest, testConstruction) {
    InMemoryStore store;
    Table t(store, Schema{ {} });

    auto ser = t.serialize();
}

TEST(TableTest, testAddCols) {
  InMemoryStore store;
  Schema schema{ {} };
  schema.christen(store);
  Table empty(store, Schema{ {} });

  empty.addCol("birthday", Cell::f(123456789.0));
  empty.addCol("name", Cell::s(""));
}
