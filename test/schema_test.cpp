#include <gtest/gtest.h>

#include "../src/schema.hpp"

TEST(SchemaTest, serde) {
  InMemoryStore store;
  Schema schema{ {} };
  schema.christen(store);

  Hashable<Schema> hash(schema.toBlob().hash);
  auto remat = hash.materialize(store);

  EXPECT_EQ(remat.getCols(), schema.getCols());
}
