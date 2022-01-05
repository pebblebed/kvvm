#include <gtest/gtest.h>
#include <string>

#include "../src/serialize.hpp"
#include "../src/dbg.hpp"

TEST(Serialize, serlz) {
  auto hStruct = HashedStruct {
    MAGIC__TABLE,
    {{ 0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf }}
  };

  std::string buf = hps::to_string(hStruct);
  HashedStruct deser;
  hps::from_string(buf, deser);
  
  dbg(ser, 0, "ser size: %zd\n", buf.size());
  EXPECT_EQ(deser.magic, MAGIC__TABLE);
  EXPECT_EQ(deser.hashen.size(), 1);
  EXPECT_EQ(deser.hashen[0], hStruct.hashen[0]);
}
