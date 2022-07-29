#ifndef ROWBANK_incl_
#define ROWBANK_incl_

#include "serialize.hpp"
#include "cell.hpp"

class RowBank : public BlobNode {
    std::vector<Row> rows_;

    public:

    size_t size() const;
    virtual Blob toBlob() const;
    static RowBank deserialize(const Blob& b);

    void append(const Row& r);
};

#endif
