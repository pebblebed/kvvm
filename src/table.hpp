#ifndef TABLE_incl_
#define TABLE_incl_

#include <vector>

#include "hash.hpp"
#include "istore.hpp"
#include "serialize.hpp"
#include "schema.hpp"
#include "cell.hpp"
#include "rowbank.hpp"

struct DataSet : public BlobInternalNode {
    typedef std::vector<Hashable<RowBank>> RowBanks;
    virtual HashedStruct flatten() const = 0;
    virtual Schema schema() const = 0;
    virtual RowBanks rows() const = 0;
};

class Table : public DataSet {
    IData &store_;
    Schema schema_;
    const RowBanks rows_;

  public:
    Table(IData &store, Schema schema, RowBanks rb = {})
    : store_(store)
    , schema_(schema)
    , rows_(rb) {
        schema_.christen(store_);
    }

    ~Table() {
    }

    virtual HashedStruct flatten() const;
    static Table deserialize(const Blob& b);

    virtual Schema schema() const;
    virtual RowBanks rows() const;

    Table addCol(std::string name, Cell defaultVal);
    Table addRow(const Row& row);
};

#endif
