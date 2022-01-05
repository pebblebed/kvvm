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

    static Cell s(std::string s) {
        return Cell {
            STRING, { 0 }, s
        };
    }

    static Cell f(float f) {
        return Cell {
            STRING, { .Float=f }, ""
        };
    }

    template <class B>
    void serialize(B& buf) const {
        buf << (int)type;
        switch (type) {
        case NUL: return;
        case STRING:
                  buf << String;
                  return;
        case BOOL:
                  buf << u.Bool;
                  return;
        case FLOAT:
                  buf << u.Float;
                  return;
        case INT:
                  buf << u.Int;
                  return;
        }

    }

    template<class B>
    void parse(B& buf) {
        int ict;
        buf >> ict;
        auto ct = ColumnType(ict);
        (ColumnType&)type = ct;
        switch(ct) {
        case NUL:
            return;
        case STRING:
            buf >> (std::string&)String;
            return;
        case BOOL:
            buf >> (bool&)u.Bool;
            return;
        case FLOAT:
            buf >> (double&)u.Float;
            return;
        case INT:
            buf >> (int64_t&)u.Int;
            return;
        }
        throw std::runtime_error("Unknown cell type");
    }

    bool operator==(const Cell& rhs) const {
        if (type != rhs.type) return false;
        if (type == STRING) return rhs.String == String;
        return rhs.u.Int == u.Int;
    }
};
struct Row {
    std::vector<Cell> cells;
};

class Table : public BlobInternalNode {
  IStore &store;
  std::vector<Hashable<RowBank>> rows;
  Hashable<Schema> schema;

  public:
  Table(IStore &store, Hashable<Schema> schema)
  : store(store)
  , schema(schema) { }
  ~Table() { }

  HashedStruct flatten() const;
  static Table deserialize(const Blob& b);

  Schema getSchema() const;
  Table addCol(std::string name, Cell defaultVal);
  Table addRow(Row row);

};

