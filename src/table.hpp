#ifndef TABLE_incl_
#define TABLE_incl_

#include <vector>

#include "hash.hpp"
#include "istore.hpp"
#include "serialize.hpp"
#include "schema.hpp"

struct Cell {
    const ColumnType type = NUL;
    typedef union {
        bool Bool;
        double Float;
        int64_t Int;
    } U;
    U u = { .Int = 0 };

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

struct DataSet : public BlobInternalNode {
    typedef std::vector<Hashable<RowBank>> RowBanks;
    virtual HashedStruct flatten() const = 0;
    virtual Schema schema() const = 0;
    virtual RowBanks rows() const = 0;
};

class Table : public DataSet {
    IStore &store_;
    RowBanks rows_;
    Hashable<Schema> schema_;

  public:
    Table(IStore &store, Hashable<Schema> schema)
    : store_(store)
    , schema_(schema) { }
    ~Table() { }

    virtual HashedStruct flatten() const;
    static Table deserialize(const Blob& b);

    virtual Schema schema() const;
    virtual RowBanks rows() const;

    Table addCol(std::string name, Cell defaultVal);
    Table addRow(Row row);
};

#endif