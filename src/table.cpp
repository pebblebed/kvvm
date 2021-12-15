#include <string>
#include <iostream>
#include <sstream>

#include "table.hpp"
#include "serialize.hpp"
#include "hps.h"

using namespace std;

Blob Schema::serialize() const {
  ostringstream out;
  hps::to_stream(schema.size(), out);
  for (auto row: schema) {
     hps::to_stream(row.first, out);
     hps::to_stream((int)row.second, out);
  }
  return Blob(out.str());
}

Schema Schema::deserialize(const Blob& b) {
  istringstream in(b.string());
  std::vector<std::pair<string, ColumnType>> schema;
  size_t sz;
  hps::from_stream(in, sz);
  for (auto i = 0; i < sz; i++) {
    string colName;
    int typeAsInt;
    hps::from_stream(in, colName);
    hps::from_stream(in, typeAsInt);
    schema.push_back(std::make_pair(colName, (ColumnType)typeAsInt));
  }
  return Schema(schema);
}



Schema
Table::getSchema() const {
  return Schema::deserialize(store.get(schema.hash));
}

Blob Table::serialize() const {
  std::vector<Hash> hashen;
  hashen.push_back(schema.hash);
  for (auto r: rows) {
    hashen.push_back(r.hash);
  }
  return serializeHashStructure(MAGIC__TABLE, hashen);
}

Table Table::addCol(string name, Cell defaultVal) {
  auto prevCols = getSchema().getCols();
  prevCols.push_back(make_pair(name, defaultVal.type));
  auto newSchema = Schema(prevCols);
  newSchema.christen(store);
  return Table(store, Hashable(newSchema));
}
