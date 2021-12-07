#include "table.hpp"
#include "serialize.hpp"

Hash Table::hash() const {
  auto ret = schema.hash;
  for (auto r: rows) {
    ret = combineOrderedHash(ret, r.hash);
  }
  return ret;
}

std::string Table::serialize() const {
  std::vector<Hash> hashen;
  hashen.push_back(schema.hash);
  return serializeHashStructure(MAGIC__TABLE, hashen);
}
