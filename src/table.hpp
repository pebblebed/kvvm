#pragma once

#include <vector>
#include <string>

#include "hash.hpp"
#include "istore.hpp"

enum ColumnType {
  STRING,
  BOOL,
  FLOAT,
  INT
};

struct Serializable {
  virtual Hash hash() const = 0;
  virtual std::string serialize() const = 0;
};

template<typename Underlying /* as Serializable */>
struct Hashable {
  Hash hash;
  Underlying materialize(IStore& store) const {
    auto s = store.get(hash);

  }
};

class Schema : public Serializable {
  std::vector<std::pair<std::string, ColumnType>> schema;

  public:

  Hash hash() const;
  std::string serialize() const;

  static Schema deserialize(std::string);
};

class RowBank : public Serializable {
  public:

  Hash hash() const;
  std::string serialize() const;

  static RowBank deserialize(std::string);
};

class Table : public Serializable {
  IStore &store;
  std::vector<Hashable<RowBank>> rows;
  Hashable<Schema> schema;

  public:
  Table(IStore &store) : store(store) { }
  ~Table() { }

  Hash hash() const;
  std::string serialize() const;
};
