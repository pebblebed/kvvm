#include <string>
#include <iostream>

#include "table.hpp"
#include "serialize.hpp"

using namespace std;

Schema
Table::schema() const {
    return Schema::deserialize(store_.get(schema_.hash));
}

HashedStruct Table::flatten() const {
  HashedStruct hs;
  hs.magic = MAGIC__TABLE;
  for (auto r: rows_) {
    hs.hashen.push_back(r.hash);
  }
  return hs;
}

Table Table::addCol(string name, Cell defaultVal) {
  auto prevCols = schema().getCols();
  prevCols.push_back(make_pair(name, defaultVal.type));
  auto newSchema = Schema(prevCols);
  newSchema.christen(store_);
  return Table(store_, Hashable<Schema>(newSchema));
}

Table Table::addRow(const Row& row) {
    Table newTab(store_, schema_);
    bool newBankNeeded = rows_.size() == 0;
    if (!newBankNeeded) {
      auto blob = store_.get(rows_[rows_.size() - 1].hash);
      auto rb = RowBank::deserialize(blob);
    }

    if (rows_.size() == 0) {
    }
    auto &banks = rows_;
    return newTab;
}

DataSet::RowBanks Table::rows() const {
    return rows_;
}

