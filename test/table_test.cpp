#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "../src/serialize.hpp"
#include "../src/istore.hpp"
#include "../src/table.hpp"
#include "../src/hps.h"

using namespace std;

TEST(TableTest, cell) {
    auto cs = Cell::s(string("abc"));
    string ser;
    hps::to_string(cs, ser);
    auto cd = hps::from_string<Cell>(ser);
    EXPECT_EQ(cs, cd);
}

TEST(TableTest, testConstruction) {
    InMemoryStore store;
    Table t(store, Schema{ {} });

    auto ser = hps::to_string(t);
}

TEST(TableTest, testAddCols) {
  InMemoryStore store;
  Schema schema{ {} };
  schema.christen(store);
  Table empty(store, Schema{ {} });

  auto newTable = empty
      .addCol("birthday", Cell::f(123456789.0))
      .addCol("name", Cell::s(""));
}
