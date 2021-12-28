#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"

TEST(Serialize, serlz) {
  auto blob = serializeHashStructure(MAGIC__TABLE, {
    { 0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf }
  });

  auto hStrResult = deserializeHashStructure(blob);
  dbg(ser, 0, "blob size: %zd\n", blob.size);
  EXPECT_EQ(hStrResult.magic, MAGIC__TABLE);
  EXPECT_EQ(hStrResult.hashen.size(), 1);
}
