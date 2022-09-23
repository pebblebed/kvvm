#include "../src/udf.hpp"
#include <gtest/gtest.h>

std::string
readpath(const char* name) {
    std::string retval;
    const size_t bufsz = 1024;
    const char buf[bufsz];
    FILE* fp = fopen(name, "r");
    while (!feof(fp)){
        size_t n = fread(buf, 1, bufsz, fp);
        retval.append(buf, n);
    }
    return retval;
}

TEST(UDFTest, testUDF) {
    MapUDF udf;
    auto args = Cell::i(7);
    auto res = udf.apply(args);
    EXPECT_EQ(res.type, ColumnType::INT);
    EXPECT_EQ(res.i, 7);
}

