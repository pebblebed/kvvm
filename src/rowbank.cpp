#include <string>
#include "rowbank.hpp"

using namespace std;

size_t
RowBank::size() const {
    return rows_.size();
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
    SerImpl::InBuffer inbuf(b.string());
    size_t max;
    decode(inbuf, max);
    RowBank retval;
    decode_vector(inbuf, retval.rows_);
    return retval;
}

void
RowBank::append(const Row& r) {
    rows_.push_back(r);
}

RowBank
RowBank::apply(std::function<Row(Row)> lambda) const {
    RowBank retval;
    for (auto r: rows_) {
        retval.append(lambda(r));
    }
    return retval;
}

