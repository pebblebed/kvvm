#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"

TEST(Serialize, serlz_int) {
    auto srz = SerImpl::to_string(0);
    EXPECT_EQ(srz.size(), 1);
    EXPECT_EQ(srz[0], '\0');

    SerImpl::InBuffer buf(srz);
    int deser = SerImpl::decode<int>(buf);
    EXPECT_EQ(deser, 0);
    
    srz = SerImpl::to_string(1);
    EXPECT_EQ(srz.size(), 2);

    SerImpl::InBuffer buf2(srz);
    deser = SerImpl::decode<int>(buf);
    EXPECT_EQ(deser, 1);
}
