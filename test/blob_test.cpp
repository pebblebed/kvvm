#include <iostream>
#include <gtest/gtest.h>
#include "../src/blob.hpp"

TEST(BlobTest, testBasics) {
    uint8_t b1[] = {
        0xde, 0xad, 0xbe, 0xef,
        0xc0, 0x01, 0xd0, 0x0d,
    };
    Blob blob1(&b1[0], sizeof(b1));
    EXPECT_EQ(blob1.hash, computeHash(b1, sizeof(b1)));
    EXPECT_EQ(blob1.size, 8);
    EXPECT_EQ(blob1.bytes[7], 0x0d);

    uint8_t b2[] = {
        0xda, 0xda, 0xf0, 0x0f,
        0x7e, 0xe3, 0xb0, 0x0b
    };
    auto blob2 = new Blob(b2, sizeof(b2));
	EXPECT_NE(blob1.hash, blob2->hash);
    EXPECT_EQ(blob2->size, 8);
    EXPECT_EQ(blob2->bytes[0], 0xda);
}

