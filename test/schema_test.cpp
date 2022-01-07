#include <gtest/gtest.h>

#include "../src/schema.hpp"

TEST(SchemaTest, serde) {
  InMemoryStore store;
  std::vector<std::pair<std::string, ColumnType>> cols {
      std::make_pair(std::string("name"), ColumnType::STRING),
  };

  Schema schema{ cols };
  schema.christen(store);

  Hashable<Schema> hash(schema.toBlob().hash);
  auto remat = hash.materialize(store);

  const auto rematCols = remat.getCols();
  const auto origCols = schema.getCols();
  EXPECT_EQ(rematCols, origCols);
  EXPECT_EQ(rematCols.size(), 1);
}
