#include "schema.hpp"
#include <sstream>

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
  for (auto i = 0; i < sz; i++) {
    std::string colName;
    int typeAsInt;
    decode(in, colName);
    decode(in, typeAsInt);
    schema.push_back(std::make_pair(colName, (ColumnType)typeAsInt));
  }
  return Schema(schema);
}

