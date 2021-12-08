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
  const ColumnType col;
  const union {
    bool Bool;
    float Float;
    int Int;
  } u;
  const std::string String;

  static Cell s(std::string s) {
    return Cell{
      STRING, { 0 }, s
    };
  }

  static Cell f(float f) {
    return Cell {
      STRING, { .Float=f }, {}
    };
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
};

template<typename Underlying /* as Serializable */>
struct Hashable {
  Hash hash;
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

  Hash hash() const;
  Blob serialize() const;

  static Schema deserialize(const Blob& b);
};

class RowBank : public Serializable {
  public:

  Hash hash() const;
  Blob serialize() const;

  static RowBank deserialize(const Blob& b);
};

class Table : public Serializable {
  IStore &store;
  std::vector<Hashable<RowBank>> rows;
  Hashable<Schema> schema;

  public:
  Table(IStore &store) : store(store) { }
  ~Table() { }

  Hash hash() const;
  Blob serialize() const;
  Schema getSchema() const;

  Table addCol(std::string name, Cell defaultVal);
  Table addRow(Row row);
};

