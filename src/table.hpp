#pragma once

#include <vector>

#include "hash.hpp"
#include "istore.hpp"

enum ColumnType {
  STRING,
  BOOL,
  FLOAT,
  INT
};

struct Cell {
  const ColumnType type;
  const union {
    bool Bool;
    float Float;
    int Int;
  } u;
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
    buf << type;
    switch (type) {
    case STRING: {
      buf << String;
      break;
    }
    case BOOL: {
      buf << u.Bool;
      break;
    }
    
    case FLOAT: {
      buf << u.Float;
      break;
    }

    case INT: {
      buf << u.Int;
      break;
    }

    default: {
      assert(false);
    }
    }
  }

};

struct Row {
  std::vector<Cell> cells;
};

struct Serializable {
  virtual Blob serialize() const = 0;

  Hash hash() const {
    return serialize().hash;
  }

  void christen(IStore& istore) const {
    auto b = serialize();
    istore.save(b);
  }
};

template<typename Underlying /* as Serializable */>
struct Hashable {
  Hash hash;

  Hashable(const Underlying& u)
  : hash(u.hash())
  { }

  Underlying materialize(IStore& store) const {
    auto s = store.get(hash);
    return Underlying::deserialize(s);
  }
};

class Schema : public Serializable {
  std::vector<std::pair<std::string, ColumnType>> schema;

  public:

  Schema(const std::vector<std::pair<std::string, ColumnType>>& cols)
  : schema(cols) { }

  std::vector<std::pair<std::string, ColumnType>> getCols() const {
    return schema;
  }
  Blob serialize() const;
  static Schema deserialize(const Blob& b);
};

class RowBank : public Serializable {
  public:

  Blob serialize() const;
  static RowBank deserialize(const Blob& b);
};

class Table : public Serializable {
  IStore &store;
  std::vector<Hashable<RowBank>> rows;
  Hashable<Schema> schema;

  public:
  Table(IStore &store, Hashable<Schema> schema)
  : store(store)
  , schema(schema) { }
  ~Table() { }

  Blob serialize() const;
  static Table deserialize(const Blob& b);

  Schema getSchema() const;
  Table addCol(std::string name, Cell defaultVal);
  Table addRow(Row row);

};

