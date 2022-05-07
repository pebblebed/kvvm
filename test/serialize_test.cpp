#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"

TEST(Serialize, serlz) {
    auto srz = SerImpl::to_string(0);
    EXPECT_EQ(srz.size(), 1);
    EXPECT_EQ(srz[0], '\0');
}
