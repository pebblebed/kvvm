#include <iostream>

#include <gtest/gtest.h>
#include "../src/istore.hpp"
#include "../src/table.hpp"

TEST(TableTest, testConstruction) {
    InMemoryStore store;
    Table t(store);
}
