#include "schema.hpp"

using namespace std;
using namespace SerImpl;

Blob Schema::toBlob() const {
  std::string outstr;
  OutBuffer out(outstr);
  encode(schema.size(), out);
  for (auto row: schema) {
    encode(row.first, out);
    encode((uint64_t)row.second, out);
  }
  return Blob(outstr);
}

Schema Schema::deserialize(const Blob& b) {
  InBuffer in(b.string());
  std::vector<std::pair<string, ColumnType>> schema;
  size_t sz;
  decode(in, sz);
  for (size_t i = 0; i < sz; i++) {
    std::string colName;
    uint64_t typeAsInt;
    decode(in, colName);
    decode(in, typeAsInt);
    schema.push_back(std::make_pair(colName, (ColumnType)typeAsInt));
  }
  return Schema(schema);
}

Schema
Schema::slice(const SliceDesc& names) const {
    std::vector<ColumnDesc> retcols;
    std::unordered_set<ColumnName> dedupedNames;
    for (const auto& n: names) {
        dedupedNames.insert(n);
    }
    for (const auto& n: dedupedNames) {
        auto it = std::find_if(schema.begin(), schema.end(), [&n](const ColumnDesc& it) {
                               return it.first == n;
                               });
        if (it == schema.end()) {
            throw std::runtime_error(std::string("no such column: ") + n);
        }
        retcols.push_back(*it);
    }
    return Schema(retcols);
}
