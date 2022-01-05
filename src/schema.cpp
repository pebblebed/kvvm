#include "schema.hpp"
#include <sstream>

using namespace std;

Blob Schema::toBlob() const {
  ostringstream out;
  out << schema.size();
  for (auto row: schema) {
     out << row.first;
     out << (int)row.second;
  }
  return Blob(out.str());
}

Schema Schema::deserialize(const Blob& b) {
  istringstream in(b.string());
  std::vector<std::pair<string, ColumnType>> schema;
  size_t sz;
  in >> sz;
  for (auto i = 0; i < sz; i++) {
    string colName;
    int typeAsInt;
    in >> colName;
    in >> typeAsInt;
    schema.push_back(std::make_pair(colName, (ColumnType)typeAsInt));
  }
  return Schema(schema);
}

