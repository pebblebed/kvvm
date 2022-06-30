<<<<<<< HEAD
#include <cstdint>

#include "serialize.hpp"
#include "rowbank.hpp"

using namespace std;

namespace SerImpl {

void encode(const Row& row, OutBuffer& b) {
    assert(false); // not implemented
}

=======
#include "rowbank.hpp"

size_t
RowBank::size() const {
    return rows_.size();
}

size_t
RowBank::max() const {
    return max_;
}

bool
RowBank::isFull() const {
    assert(size() <= max_);
    return size() == max_;
>>>>>>> 5ee66cd (Inroads to tabular append.)
}

Blob
RowBank::toBlob() const {
<<<<<<< HEAD
    using namespace SerImpl;
    string s;
    OutBuffer out(s);
    encode_vector(rows_, out);
    return Blob(s);
=======
    std::string data;
    SerImpl::OutBuffer outbuf(data);
    encode(max_, outbuf);
    encode_vector(rows_, outbuf);
    return Blob(data);
>>>>>>> 5ee66cd (Inroads to tabular append.)
}

RowBank
RowBank::deserialize(const Blob& b) {
<<<<<<< HEAD
    RowBank rb;

    return rb;
}
=======
    SerImpl::InBuffer inbuf(b.string());
    size_t max;
    decode(inbuf, max);
    RowBank retval(max);
    decode_vector(inbuf, retval.rows_);
    return retval;
}

RowBank*
RowBank::append(const Row& r) {
    if (isFull()) {
        auto ret = new RowBank(max());
        ret->rows_.push_back(r);
        return ret;
    }
    rows_.push_back(r);
    return nullptr;
}

>>>>>>> 5ee66cd (Inroads to tabular append.)
