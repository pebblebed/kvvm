#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <math.h>

#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <bit>

#include "serialize.hpp"
#include "hps.h"
#include "dbg.hpp"

// This space left blank

namespace SerImpl {

int clz(uint64_t x) {
    return __builtin_clzl(x);
}

void encode(uint8_t u8, OutBuffer& b) { b.produce(u8); }
void encode(bool boo, OutBuffer& b)   { b.produce(boo); }


void encode(uint64_t u64, OutBuffer& b) {
    if (u64 == 0) { b.produce(0); return; }

    auto maxBits = sizeof(uint64_t) * 8;
    int leadingZeros = clz(u64);
    assert(leadingZeros < maxBits);
    auto nonZeroBytes = sizeof(u64) - leadingZeros / 8;
    assert(nonZeroBytes > 0);
    assert(nonZeroBytes < UINT8_MAX);
    b.produce(nonZeroBytes);
    for (auto ii = 0; ii < nonZeroBytes; ii++) {
        b.produce( (u64 >> 8 * ii) & 0xff);
	}
}

// Argh, sign. Just encode as unsigned, so that negative numbers are annoyingly
// long to represent.
void encode(int64_t i64, OutBuffer& b) {
    encode(uint64_t(i64), b);
}

void encode(std::string s, OutBuffer& b) {
    encode(s.size(), b);
    for (auto c: s) {
        b.produce(c);
    }
}

// Yikes! Serializing ieee floating point. Presumably, like ints, many numbers are small, so
// break them into structs and serialize them as small integers. Probably takes <8 bytes to
// represent most.
struct CanonFloat {
    bool neg;
    int64_t exp;
    uint64_t fraction;
};

CanonFloat toCF(double d64) {
    bool neg = d64 < 0.0;
    if (neg) d64 -= d64;
    int exp = 0;
    auto dSignif = frexp(d64, &exp);
    uint64_t fraction = (int64_t)(dSignif * double(1 << exp));
    return { neg, exp, fraction };
}

double fromCF(CanonFloat cf) {
    double retval = double(cf.fraction) * (1 << cf.exp);
    if (cf.neg) return -retval;
    return retval;
}

void encode(double d64, OutBuffer& b) {
    auto cf = toCF(d64);
    encode(cf.neg, b);
    encode(uint64_t(cf.exp), b);
    encode(cf.fraction, b);
}

void decode(InBuffer& b, double& d64) {
    CanonFloat cf;
    decode(b, cf.neg);
    decode(b, cf.exp);
    decode(b, cf.fraction);
    d64 = fromCF(cf);
}

void decode(InBuffer& b, uint64_t& u64) {
    auto nBytes = b.consume();
    assert(nBytes >= 0 && nBytes <= 8);
    u64 = 0;
    for (auto ii = 0; ii < nBytes; ii++) {
        u64 |= (b.consume() << 8 * ii);
    }
}

void decode(InBuffer& b, int64_t& i64) {
    decode(b, (uint64_t&)i64);
}

void decode(InBuffer& b, bool& boo) {
    auto byte = b.consume();
    assert(byte == 0 || byte == 1);
    boo = bool(byte);
}

void decode(InBuffer& b, uint8_t& byte) {
    byte = b.consume();
}

void decode(InBuffer& b, std::string& str) {
    size_t sz;
    decode(b, sz);
    str.resize(0);
    str.reserve(sz);
    for (size_t ii = 0; ii < sz; ii++) {
        str.push_back(char(b.consume()));
    }
}

void OutBuffer::produce(uint8_t b) {
        buf.resize(buf.size() + 1);
        buf[buf.size() - 1] = b;
}


uint8_t InBuffer::consume() {
    return uint8_t(*it++);
}

} // namespace SerImpl
