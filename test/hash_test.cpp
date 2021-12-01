#include <iostream>

#include <gtest/gtest.h>
#include "../src/hash.hpp"

TEST(HashTest, testHex) {
   Hash hr;
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
    HashState hr1, hr2;
    uint64_t salt = 0xdeadbeefc001d00d;
    auto h1 = hr1(reinterpret_cast<uint8_t*>(&salt), sizeof(salt));
    salt++;
    auto h2 = hr2(reinterpret_cast<uint8_t*>(&salt), sizeof(salt));
    std::cerr << h1.hex() << "\n" << h2.hex() << "\n";
    EXPECT_NE(h1, h2);
}
