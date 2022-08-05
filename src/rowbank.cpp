#include <string>
#include "rowbank.hpp"

using namespace std;

size_t
RowBank::size() const {
    return rows_.size();
}

const Row&
RowBank::at(int i) const {
    assert(i <= size());
    return rows_[i];
}

Blob
RowBank::toBlob() const {
    using namespace SerImpl;
    string s;
    OutBuffer out(s);
    encode(rows_, out);
    return Blob(s);
}

RowBank
RowBank::deserialize(const Blob& b) {
    using namespace SerImpl;
    std::string s = b.string();
    InBuffer inbuf(s);
    RowBank retval;
    decode(inbuf, retval.rows_);
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

