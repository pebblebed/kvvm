#include <string>
#include <cassert>

#include <openssl/evp.h> //for all other OpenSSL function calls
#include <openssl/sha.h> //for SHA512_DIGEST_LENGTH

#include "hash.hpp"

using namespace std;

char nybbleToC(uint8_t nybble) {
    assert(nybble >= 0);
    assert(nybble < 16);
    if (nybble < 10) return '0' + nybble;
    return 'a' + (nybble - 10);
}

string HashResult::hex() const {
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

void HashState::operator()(const uint8_t* bytes, size_t size) {
    uint32_t digest_length = SHA512_DIGEST_LENGTH;
    assert(SHA512_DIGEST_LENGTH <= HASH_BITS);
    const EVP_MD* algorithm = EVP_sha3_512();
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    EVP_DigestInit_ex(context, algorithm, nullptr);
    EVP_DigestUpdate(context, (unsigned char*)bytes, size);
    EVP_DigestFinal_ex(context, (unsigned char*)&state.bytes, &digest_length);
    EVP_MD_CTX_destroy(context);
}

