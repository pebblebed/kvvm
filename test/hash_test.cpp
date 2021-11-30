#include <gtest/gtest.h>
#include "../src/hash.hpp"

TEST(HashTest, testHex) {
   HashResult hr;
   auto pretty = hr.hex();
   for (auto c : pretty) {
      EXPECT_EQ(c, '0');
   }
}
