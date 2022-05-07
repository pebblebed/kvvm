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

std::string to_string(const HashedStruct& hs) {
    // Soon!
    assert(false);
}

}
