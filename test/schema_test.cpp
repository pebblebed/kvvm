#include <gtest/gtest.h>

#include "../src/schema.hpp"
#include "../src/dbg.hpp"

TEST(SchemaTest, serde) {
    auto data = getTestData();
    std::vector<std::pair<std::string, ColumnType>> cols {
        std::make_pair(std::string("name"), ColumnType::STRING),
    };

    Schema schema{ cols };
    schema.christen(*data);

    Hashable<Schema> hash(schema.toBlob().hash);
    auto remat = hash.materialize(*data);
    dbg(ser, 0, "blobinz: %s\n", schema.toBlob().hash.hex().c_str());

    const auto rematCols = remat.getCols();
    const auto origCols = schema.getCols();
    EXPECT_EQ(rematCols, origCols);
    EXPECT_EQ(rematCols.size(), 1);
}

TEST(SchemaTest, slice) {
    auto cols = {
        std::make_pair(std::string("name"), ColumnType::STRING),
        std::make_pair(std::string("postCode"), ColumnType::INT),
    };
    Schema schema{ cols };

    auto newSchema = schema.slice( { std::string("postCode") } );
    auto newCols = newSchema.getCols();
    EXPECT_EQ(newCols.size(), 1);
    EXPECT_EQ(newCols[0].first, std::string("postCode"));
    EXPECT_EQ(newCols[0].second, ColumnType::INT);
}
