#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"
#include "../src/cell.hpp"

using namespace SerImpl;

TEST(Serialize, serlz_int_0) {
    std::string data;
    OutBuffer out(data);
    encode(uint64_t(0), out);
    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], '\0');

    SerImpl::InBuffer in(data);
    uint64_t deser;
    decode(in, deser);
    EXPECT_EQ(deser, 0);
}

TEST(Serialize, serlz_int_larger) {
    std::string data;
    OutBuffer out(data);
    encode(uint64_t(1), out);
    EXPECT_EQ(data.size(), 2);

    InBuffer in(data);
    uint64_t deser;
    decode(in, deser);
    EXPECT_EQ(deser, 1);
}

TEST(Serialize, serlz_cell_s) {
    std::string data;
    OutBuffer out(data);
    auto s = Cell::s("foo");
    s.encode(out);
    EXPECT_EQ(data.size(), 6);

    InBuffer in(data);
    auto c = Cell::decode(in);
    EXPECT_EQ(c.type, ColumnType::STRING);
    EXPECT_EQ(c.String, std::string("foo"));
}