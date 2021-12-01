#include <string>
#include <cassert>

#include <openssl/evp.h>
#include <openssl/sha.h>

#include "hash.hpp"

using namespace std;


char nybbleToC(uint8_t nybble) {
    assert(nybble >= 0);
    assert(nybble < 16);
    if (nybble < 10) return '0' + nybble;
    return 'a' + (nybble - 10);
}


string Hash::hex() const {
	string retval;
    const int BITS_PER_HEX_CHAR = 4;
	retval.resize(HASH_BITS / BITS_PER_HEX_CHAR);
	for (auto ii = 0; ii < HASH_BYTES; ii++) {
        auto c = bytes[ii];
        retval[ii * 2] = nybbleToC(c >> 4);
        retval[ii * 2 + 1] = nybbleToC(c & 0xf);
    }
	return retval;
}


bool Hash::operator==(const Hash& rhs) const {
    return memcmp(bytes, rhs.bytes, HASH_BYTES) == 0;
}


bool Hash::operator!=(const Hash& rhs) const {
    return !(*this == rhs);
}

HashState::HashState() {
    assert(SHA512_DIGEST_LENGTH <= HASH_BITS);
    algorithm = EVP_sha3_512();
    context = EVP_MD_CTX_new();
    EVP_DigestInit_ex(context, algorithm, nullptr);
}

HashState::~HashState() {
    EVP_MD_CTX_destroy(context);
}

Hash HashState::operator()(const uint8_t* bytes, size_t size) {
    Hash state;
    EVP_DigestUpdate(context, (unsigned char*)bytes, size);
    EVP_DigestFinal_ex(context, (unsigned char*)state.bytes, nullptr);
    return state;
}


Hash computeHash(const uint8_t* bytes, size_t sz) {
    HashState hs;
    return hs(bytes, sz);
}
