#ifndef SCHEMA_incl_
#define SCHEMA_incl_

#include <vector>
#include <unordered_set>
#include <algorithm>
#include "blob.hpp"
#include "serialize.hpp"
#include "cell.hpp"

// A Schema is a leaf, with its own full-custom treatment of 
class Schema : public BlobNode {
    std::vector<ColumnDesc> schema;

    public:

    Schema(const std::vector<ColumnDesc>& cols)
        : schema(cols) { }

    std::vector<ColumnDesc> getCols() const {
        return schema;
    }
    virtual Blob toBlob() const;
    static Schema deserialize(const Blob& b);
    Hash hash() const {
        return toBlob().hash;
    }

    Schema slice(const SliceDesc& slice) const; 
    std::vector<int> sliceToColNums(const SliceDesc& slice) const;

    struct TypeMismatch {
        std::string name;
        ColumnType expected;
        ColumnType found;
    };

    std::vector<TypeMismatch> check(const std::vector<Cell>& cells) const;
    Schema join(const Schema& other) const {
        std::vector<ColumnDesc> newCols = schema;
        newCols.insert(newCols.end(), other.schema.begin(), other.schema.end());
        return Schema(newCols);
    }
};

#endif
