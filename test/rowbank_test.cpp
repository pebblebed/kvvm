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
