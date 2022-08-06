#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "../src/serialize.hpp"
#include "../src/istore.hpp"
#include "../src/table.hpp"
#include "../src/query.hpp"

TEST(QueryTest, slice) {
    auto d = getTestData();
    Table empty { *d, Schema{ {} } };
    auto newTab = empty
        .addCol("user", Cell::s("kma"))
        .addCol("name", Cell::s("Keith Adams"))
        .addCol("birth", Cell::f(12345678.0))

        .addRow(Row { {
            Cell::s("pam"), Cell::s("Pamela Vagata"), Cell::f(12345688.0) } } )
        .addRow(Row { {
            Cell::s("emmett"), Cell::s("Emmett Adams"), Cell::f(12345698.0) } } );

       
}
