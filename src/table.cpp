#include "table.hpp"
#include "serialize.hpp"

//// Schema
#if 0
Blob Schema::serialize() {
  std::string ret;
  return Blob(s);
}

Schema Schema::deserialize(const Blob& b) {
  
}
#endif


//// Table

Hash Table::hash() const {
  auto ret = schema.hash;
  for (auto r: rows) {
    ret = combineOrderedHash(ret, r.hash);
  }
  return ret;
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

Table Table::addCol(std::string name, Cell defaultVal) {
  auto curSchema = getSchema();
}
