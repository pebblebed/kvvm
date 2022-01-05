#include <string>
#include <iostream>

#include "table.hpp"
#include "serialize.hpp"
#include "hps.h"

using namespace std;

Schema
Table::getSchema() const {
  return Schema::deserialize(store.get(schema.hash));
}

HashedStruct Table::flatten() const {
  HashedStruct hs;
  hs.magic = MAGIC__TABLE;
  for (auto r: rows) {
    hs.hashen.push_back(r.hash);
  }
  return hs;
}

Table Table::addCol(string name, Cell defaultVal) {
  auto prevCols = getSchema().getCols();
  prevCols.push_back(make_pair(name, defaultVal.type));
  auto newSchema = Schema(prevCols);
  newSchema.christen(store);
  return Table(store, Hashable(newSchema));
}
