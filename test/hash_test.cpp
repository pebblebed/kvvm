#include <iostream>

#include <gtest/gtest.h>
#include "../src/hash.hpp"

TEST(HashTest, testHex) {
   HashResult hr;
   static const size_t OUTPUT_BYTES = HASH_BYTES * 2;
   auto pretty = hr.hex();
   EXPECT_EQ(pretty.size(), OUTPUT_BYTES);
   for (auto c : pretty) {
      EXPECT_EQ(c, '0');
   }

   memset(hr.bytes, 0xf1, sizeof(hr.bytes));
   pretty = hr.hex();
   for (int ii = 0; ii < OUTPUT_BYTES; ii++) {
       auto c = pretty[ii];
       if ((ii % 2) == 0) {
           EXPECT_EQ(c, 'f');
           continue;
       }
       EXPECT_EQ(c, '1');
   }
}

TEST(HashTest, testHash) {
    HashState hr;
    uint64_t salt = 0xdeadbeefc001d00d;
    hr(reinterpret_cast<uint8_t*>(&salt), sizeof(salt));
    std::cerr << hr.currentState().hex() << "\n";
}
