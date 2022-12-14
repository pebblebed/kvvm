#include <gtest/gtest.h>
#include "../src/rowbank.hpp"

using namespace std;

TEST(RowBankTest, ctor) {
    RowBank rb1;
    EXPECT_EQ(rb1.size(), 0);
}

TEST(RowBankTest, contentAddr) {
    RowBank rb1, rb2;
    // The empty rowbank is everywhere identical
    EXPECT_EQ(rb1.hash(), rb2.hash());

    // Hashing is column-order-sensitive.
    auto r1 = Row { { Cell::s("foo"), Cell::s("bar") } };
    auto r2 = Row { { Cell::s("bar"), Cell::s("foo") } };
    rb1.append(r1); 
    rb2.append(r2);
    EXPECT_NE(rb1.hash(), rb2.hash());

    // Hashing agrees for non-empty items
    {
        RowBank rb1, rb2;
        rb1.append(r1);
        rb1.append(r2);

        rb2.append(r1);
        rb2.append(r2);

        EXPECT_EQ(rb1.hash(), rb2.hash());
    }
}

TEST(RowBankTest, apply) {
    RowBank rb1;
    for (auto i = 0; i < 420; i++) {
        rb1.append(Row { { Cell::s("a string"), Cell::i(i) } });
    }

    std::function<Row(Row)> sliceLeft = [](Row r) {
        EXPECT_EQ(r.cells.size(), 2);
        return Row { { r.cells[0] } };
    };
    RowBank rb2 = rb1.apply(sliceLeft);

    // Now check!
    std::function<Row(Row)> verify = [](Row r) {
        EXPECT_EQ(r.cells.size(), 1);
        EXPECT_EQ(r.cells[0], Cell::s("a string"));
        return Row { };
    };
    auto rb3 = rb2.apply(verify);

    // Compute some stuff
    std::function<Row(Row)> spliceSquares = [](Row r) {
        EXPECT_EQ(r.cells.size(), 2);
        EXPECT_EQ(r.cells[1].type, ColumnType::INT);
        auto sq = r.cells[1].u.Int;
        sq = sq * sq;
        return Row { {
            r.cells[0], r.cells[1], Cell::i(sq),
        } };
    };

    auto rb4 = rb1.apply(spliceSquares);
    EXPECT_EQ(rb4.at(100).cells[2], Cell::i(100 * 100));
}

TEST(RowBankTest, serde) {
    using namespace SerImpl;

    RowBank rb1;
    for (auto i = 0; i < 2; i++) {
        rb1.append(Row { { Cell::s("fooble"), Cell::i(i * 3) } } );
    }
    auto blob = rb1.toBlob();

    RowBank rb2 = RowBank::deserialize(blob);
}

