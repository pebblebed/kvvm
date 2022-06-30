#ifndef TABLE_incl_
#define TABLE_incl_

#include <vector>

#include "hash.hpp"
#include "istore.hpp"
#include "serialize.hpp"
#include "schema.hpp"
#include "cell.hpp"

class RowBank : public BlobNode {
    Schema schema_;
    std::vector<Row> rows_;

  public:

  virtual Blob toBlob() const;
  static RowBank deserialize(const Blob& b);
};

struct DataSet : public BlobInternalNode {
    typedef std::vector<Hashable<RowBank>> RowBanks;
    virtual HashedStruct flatten() const = 0;
    virtual Schema schema() const = 0;
    virtual RowBanks rows() const = 0;
};

class Table : public DataSet {
    IStore &store_;
    Hashable<Schema> schema_;
    const RowBanks rows_;

  public:
    Table(IStore &store, Hashable<Schema> schema, RowBanks rb = {})
    : store_(store)
    , schema_(schema)
    , rows_(rb) { }
    ~Table() { }

    virtual HashedStruct flatten() const;
    static Table deserialize(const Blob& b);

    virtual Schema schema() const;
    virtual RowBanks rows() const;

    Table addCol(std::string name, Cell defaultVal);
    Table addRow(const Row& row);
};

#endif
