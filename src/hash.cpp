#include <string>
#include <cassert>

#include "hash.hpp"

using namespace std;

char nybbleToC(uint8_t nybble) {
    assert(nybble >= 0);
    assert(nybble < 16);
    if (nybble < 10) return '0' + nybble;
    return 'a' + (nybble - 10);
}

string HashResult::hex() {
	string retval;
	retval.resize(HASH_LENGTH * 2);
	for (auto ii = 0; ii < HASH_LENGTH; ii++) {
        auto c = bytes[ii];
        retval[ii * 2] = nybbleToC(c >> 4);
        retval[ii * 2 + 1] = nybbleToC(c & 0xf);
    }
	return retval;
}

void HashState::operator()(const uint8_t* bytes, size_t sz) {

}

