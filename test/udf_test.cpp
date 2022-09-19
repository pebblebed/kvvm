#include "../src/udf.hpp"
#include <gtest/gtest.h>

TEST(UDFTest, testUDF) {
    MapUDF udf;
    auto args = Cell::i(7);
    auto res = udf.apply(args);
    EXPECT_EQ(res.type, ColumnType::INT);
    EXPECT_EQ(res.i, 7);
}

