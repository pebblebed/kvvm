#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "../src/serialize.hpp"
#include "../src/istore.hpp"
#include "../src/table.hpp"

using namespace std;

TEST(TableTest, cell) {
    auto cs = Cell::s(string("abc"));
}

TEST(TableTest, testConstruction) {
    InMemoryStore store;
    Table t(store, Schema{ {} });
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

TEST(TableTest, testAddRows) {
    InMemoryStore store;
    Schema schema{ {} };
    schema.christen(store);
    Table empty(store,
                Schema {
                { { std::string("name"), ColumnType::STRING },
                { std::string("height"), ColumnType::INT } },
                });
    Row r { { Cell::s("Keith Adams"), Cell::i(69) } };
    empty.addRow(r);
}
