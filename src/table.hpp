#pragma once

#include <vector>

#include "hash.hpp"
#include "istore.hpp"
#include "serialize.hpp"
#include "schema.hpp"

struct Cell {
    const ColumnType type = NUL;
    const union {
        bool Bool;
        double Float;
        int64_t Int;
    } u = { .Int = 0 };

    const std::string String;

    static Cell null();
    static Cell s(std::string s);
    static Cell b(bool b);
    static Cell f(float f);
    static Cell i(int64_t i64);
    static Cell decode(SerImpl::InBuffer& buf);

    void encode(SerImpl::OutBuffer& buf) const;

    bool operator==(const Cell& rhs) const;
};
struct Row {
    std::vector<Cell> cells;
};

class DataSet : public BlobInternalNode {
    virtual HashedStruct flatten() const = 0;
    virtual Schema getSchema() const = 0;
};

class Table : public DataSet {
  IStore &store;
  std::vector<Hashable<RowBank>> rows;
  Hashable<Schema> schema;

  public:
  Table(IStore &store, Hashable<Schema> schema)
  : store(store)
  , schema(schema) { }
  ~Table() { }

  virtual HashedStruct flatten() const;
  static Table deserialize(const Blob& b);

  virtual Schema getSchema() const;
  
  Table addCol(std::string name, Cell defaultVal);
  Table addRow(Row row);
};

// A Slice is a subset of the columns of a dataset
class Slice: public DataSet {
    Slice(const DataSet& underlying, const Schema& slice);
    virtual HashedStruct flatten() const;
    virtual Schema getSchema() const;
};
