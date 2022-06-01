#pragma once

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

    Schema slice(std::vector<ColumnName> names) {
      std::vector<ColumnDesc> retcols;
      std::unordered_set<ColumnName> dedupedNames;
      for (const auto& n: names) {
        dedupedNames.insert(n);
      }
      for (const auto& n: dedupedNames) {
        auto it = std::find_if(schema.begin(), schema.end(), [&n](const ColumnDesc& it) {
          return it.first == n;
        });
        if (it == schema.end()) {
          throw std::runtime_error(std::string("no such column: ") + n);
        }
        retcols.push_back(*it);
      }
      return Schema(retcols);
    }
};

class RowBank : public BlobNode {
  public:

  virtual Blob toBlob() const;
  static RowBank deserialize(const Blob& b);
};

