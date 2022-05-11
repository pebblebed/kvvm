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
        using namespace SerImpl;
        encode((int)type, buf);
        switch (type) {
        case NUL: return;
        case STRING:
                  encode(String, buf);
                  return;
        case BOOL:
                  encode(u.Bool, buf);
                  return;
        case FLOAT:
                  encode(u.Float, buf);
                  return;
        case INT:
                  encode(u.Int, buf);
                  return;
        }

    }

    void decode(SerImpl::InBuffer& buf) {
        size_t ict;
        SerImpl::decode(buf, ict);
        auto ct = ColumnType(ict);
        (ColumnType&)type = ct;
        switch(ct) {
        case NUL:
            return;
        case STRING:
            SerImpl::decode(buf, (std::string&)String);
            return;
        case BOOL:
            SerImpl::decode(buf, u.Bool);
            return;
        case FLOAT:
            SerImpl::decode(buf, u.Float);
            return;
        case INT:
            SerImpl::decode(buf, u.Int);
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

