#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"

TEST(Serialize, serlz) {
  auto blob = serializeHashStructure(MAGIC__TABLE, {
    { 0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf }
  });
  EXPECT_EQ(blob.size, MAGIC_BYTES + sizeof(size_t) + sizeof(Hash));
  EXPECT_EQ(blob.substr(0, MAGIC_BYTES), std::string("_TABLE"));
}
