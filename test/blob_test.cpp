#include <gtest/gtest.h>
#include "../src/blob.hpp"

TEST(BlobTest, testBasics) {
    uint8_t b1[] = {
        0xde, 0xad, 0xbe, 0xef,
        0xc0, 0x01, 0xd0, 0x0d,
    };
    auto b = new Blob(b1, sizeof(b1));
}

