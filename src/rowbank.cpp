#include <cstdint>

#include "serialize.hpp"
#include "rowbank.hpp"

using namespace std;

namespace SerImpl {

void encode(const Row& row, OutBuffer& b) {
    assert(false); // not implemented
}

}

Blob
RowBank::toBlob() const {
    using namespace SerImpl;
    string s;
    OutBuffer out(s);
    encode_vector(rows_, out);
    return Blob(s);
}

RowBank
RowBank::deserialize(const Blob& b) {
    RowBank rb;

    return rb;
}
