#ifndef SCHEMA_incl_
#define SCHEMA_incl_

#include <vector>
#include <unordered_set>
#include <algorithm>
#include "blob.hpp"
#include "serialize.hpp"

enum ColumnType {
  NUL,
  STRING,
  BOOL,
  FLOAT,
  INT
};

typedef std::string ColumnName;
typedef std::pair<ColumnName, ColumnType> ColumnDesc;

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

    Schema slice(std::vector<ColumnName> names) const; 
};

class RowBank : public BlobNode {
  public:

  virtual Blob toBlob() const;
  static RowBank deserialize(const Blob& b);
};

#endif
