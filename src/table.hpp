#pragma once

#include <vector>
#include <string>

#include "hash.hpp"

enum ColumnType {
  STRING,
  BOOL,
  FLOAT,
  INT
};

class Schema {
  std::vector<std::pair<std::string, ColumnType>> schema;

  public:

  Hash hash() const;
  std::string serialize() const;

  static Schema deserialize(std::string);
};

class Table {
  std::vector<Hash> rows;
  Hash schema;

  public:

  Hash hash() const;
  std::string serialize() const;
};
