#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"
#include "../src/cell.hpp"

using namespace SerImpl;
using namespace std;

template<typename TInner>
void
SerCase(const TInner val) {
    string data;
    OutBuffer out(data);
    encode(val, out);

    InBuffer in(data);
    TInner deser;
    decode(in, deser);
    EXPECT_EQ(deser, val);
}

TEST(Serialize, serlz_int_0) {
    SerCase(uint64_t(0));
}

TEST(Serialize, serlz_int_larger) {
    SerCase(uint64_t(1));
}

TEST(Serialize, serlz_dense_ints) {
    uint64_t prod = uint64_t(rand()) * uint64_t(rand());
    SerCase(prod);
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

TEST(Serialize, vecvec) {
    const int X = 2;
    const int Y = 3;

    std::vector<std::vector<uint64_t>> vecvec;
    for (auto i = 0; i < X; i++) {
        std::vector<uint64_t> inner;
        for (auto j = 0; j < Y; j++) {
            inner.push_back(uint64_t(rand()) * uint64_t(rand()));
        }
        vecvec.push_back(inner);
    }

    string outStr;
    OutBuffer outb(outStr);
    encode(vecvec, outb);

    InBuffer inb(outStr);
    std::vector<std::vector<uint64_t>> decvec;
    decode(inb, decvec);

    EXPECT_EQ(decvec.size(), X);
    EXPECT_EQ(decvec[0].size(), Y);

    EXPECT_EQ(vecvec, decvec);
}
