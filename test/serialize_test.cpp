#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"

TEST(Serialize, serlz_int_0) {
    using namespace SerImpl;
    std::string data;
    OutBuffer out(data);
    encode(uint64_t(0), out);
    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], '\0');

    SerImpl::InBuffer in(data);
    int deser;
    decode(in, deser);
    EXPECT_EQ(deser, 0);
}

TEST(Serialize, serlz_int_larger) {
    using namespace SerImpl;
    std::string data;
    OutBuffer out(data);
    encode(uint64_t(1), out);
    EXPECT_EQ(data.size(), 2);

    InBuffer in(data);
    int deser;
    decode(in, deser);
    EXPECT_EQ(deser, 1);
}
