#include "table.hpp"
#include "serialize.hpp"
#include "hps.h"

using namespace std;

Blob Schema::serialize() const {
  string s;
  hps::to_string(schema.size(), s);
  for (auto row: schema) {
     hps::to_string(row.first, s);
     hps::to_string((int)row.second, s);
  }
  return Blob(s);
}

Schema Schema::deserialize(const Blob& b) {
  std::vector<std::pair<string, ColumnType>> schema;
  auto s = b.string();
  size_t sz;
  hps::from_string(s, sz);
  for (auto i = 0; i < sz; i++) {
    string colName;
    int typeAsInt;
    hps::from_string(s, colName);
    hps::from_string(s, typeAsInt);
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
  auto curSchema = getSchema();
  auto newSchema = curSchema;
  return Table(store, Hashable(newSchema));
}
