#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>

#include "serialize.hpp"
#include "hps.h"
#include "dbg.hpp"

// This space left blank

namespace SerImpl {

template<> int clz<unsigned>(unsigned x) {
    return __builtin_clz(x);
}

template<> int clz<int>(int i) {
    return clz<unsigned>(unsigned(i));
}


template<> int clz<unsigned long>(unsigned long x) {
    return __builtin_clzl(x);
}

template<> int clz<unsigned long long>(unsigned long long x) {
    return __builtin_clzll(x);
}

void encode(std::string s, OutBuffer b) {
    encode_int_impl<uint64_t>(s.size(), b);
    for (auto c: s) {
        b.produce(c);
    }
}

void encode(uint64_t u64, OutBuffer b) { encode_int_impl<uint64_t>(u64, b); }
void encode(uint8_t u8, OutBuffer b) { b.produce(u8); }

std::string to_string(const HashedStruct& hs) {
    // Soon!
    assert(false);
}

void OutBuffer::produce(uint8_t b) {
        buf.resize(buf.size() + 1);
        buf[buf.size() - 1] = b;
}


uint8_t InBuffer::consume() {
    return uint8_t(*it++);
}

} // namespace
