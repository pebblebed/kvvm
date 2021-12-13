#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"

TEST(Serialize, serlz) {
  auto blob = serializeHashStructure(MAGIC__TABLE, {
    { 0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf }
  });

  auto hStrResult = deserializeHashStructure(blob);
}
