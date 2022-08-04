#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"
#include "../src/cell.hpp"

using namespace SerImpl;
using namespace std;

TEST(Serialize, serlz_int_0) {
    string data;
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
    string data;
    OutBuffer out(data);
    encode(uint64_t(1), out);
    EXPECT_EQ(data.size(), 2);

    InBuffer in(data);
    uint64_t deser;
    decode(in, deser);
    EXPECT_EQ(deser, 1);
}

TEST(Serialize, serlz_cell_s) {
    string data;
    OutBuffer out(data);
    auto s = Cell::s("foo");
    s.encode(out);
    EXPECT_EQ(data.size(), 6);

    InBuffer in(data);
    auto c = Cell::decode(in);
    EXPECT_EQ(c.type, ColumnType::STRING);
    EXPECT_EQ(c.String, string("foo"));
}

TEST(Serialize, hash) {
    uint8_t bytes[5] = { 0xb, 0x1, 0xa, 0xd, 0xe };
    auto h = computeHash(bytes, 5);
    string s;
    OutBuffer buf(s);
    encode(h, buf);

    InBuffer inb(s);
    Hash h2;
    decode(inb, h2);
}

TEST(Serialize, vector) {
    std::vector<uint64_t> veccio;
    for (auto i = 0; i < 62; i++) {
        veccio.push_back(1 << i + i);
    }

    string outStr;
    OutBuffer buf(outStr);
    encode(veccio, buf);

    InBuffer inb(outStr);
    std::vector<uint64_t> decvec;
    decode(inb, decvec);

    EXPECT_EQ(decvec.size(), 62);
    for (auto i = 0; i < decvec.size(); i++) {
        EXPECT_EQ(decvec[i], veccio[i]);
    }
}
