#pragma once

#include "blob.hpp"
#include "serialize.hpp"

enum ColumnType {
  NUL,
  STRING,
  BOOL,
  FLOAT,
  INT
};

// A Schema is a leaf, with its own full-custom treatment of 
class Schema : public BlobNode {
    std::vector<std::pair<std::string, ColumnType>> schema;

    public:

    Schema(const std::vector<std::pair<std::string, ColumnType>>& cols)
        : schema(cols) { }

    std::vector<std::pair<std::string, ColumnType>> getCols() const {
        return schema;
    }
    virtual Blob toBlob() const;
    static Schema deserialize(const Blob& b);
    Hash hash() const {
        return toBlob().hash;
    }
};

class RowBank : public BlobNode {
  public:

  virtual Blob toBlob() const;
  static RowBank deserialize(const Blob& b);
};

