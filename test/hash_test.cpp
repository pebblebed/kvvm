#include <gtest/gtest.h>
#include "../src/hash.hpp"

TEST(HashTest, testHex) {
   HashResult hr;
   auto pretty = hr.hex();
   for (auto c : pretty) {
      EXPECT_EQ(c, '0');
   }

   memset(hr.bytes, 0xf1, sizeof(hr.bytes));
   pretty = hr.hex();
   for (int ii = 0; ii < HASH_LENGTH * 2; ii++) {
       auto c = pretty[ii];
       if ((ii % 2) == 0) {
           EXPECT_EQ(c, 'f');
           continue;
       }
       EXPECT_EQ(c, '1');
   }
}
