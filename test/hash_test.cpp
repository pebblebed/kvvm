#include <iostream>
#include <sstream>

#include <gtest/gtest.h>
#include "../src/hash.hpp"
#include "../src/hps.h"

TEST(HashTest, testHex) {
   Hash hr;
   static const size_t OUTPUT_BYTES = HASH_BYTES * 2;
   auto pretty = hr.hex();
   EXPECT_EQ(pretty.size(), OUTPUT_BYTES);
   for (auto c : pretty) {
      EXPECT_EQ(c, '0');
   }

   memset(&hr.bytes[0], 0xf1, sizeof(hr.bytes));
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
    uint64_t salt = 0xdeadbeefc001d00d;
    auto ch = [&]() {
        return computeHash(reinterpret_cast<uint8_t*>(&salt), sizeof(salt));
    };
    auto h1 = ch();
    auto h2 = ch();
    EXPECT_EQ(h1, h2);
    salt++;
    auto h3 = ch();
    EXPECT_NE(h1, h3);
}

TEST(HashTest, serialize) {
    auto str = "computing is fun!";
    const auto strLen = strlen(str);
    const auto target = computeHash((const uint8_t*)str, strLen);

    std::string ser;
    hps::to_string(target, ser);

    auto result = hps::from_string<Hash>(ser);
    EXPECT_EQ(result, target);
}
