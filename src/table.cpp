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
    newTab.rows_ = rows_;
    RowBank* maybeNewRB = nullptr;
    if (rows_.size() == 0) {
        maybeNewRB = new RowBank();
    } else {
        auto tailHash = newTab.rows_[newTab.rows_.size() - 1].hash;
        auto tailBlob = store_.get(tailHash);
        auto tailRB = RowBank::deserialize(tailBlob);
        if (tailRB.isFull()) {
            maybeNewRB = new RowBank();
        }
    }
    if (maybeNewRB) {
        newTab.rows_.push_back(*maybeNewRB);
        // Just get the happy-path body inlined recursively
        auto retval = newTab.addRow(row);
        delete maybeNewRB;
        return retval;
    }
    // OK! Preliminaries out of the way: we have space allocated for the new row.
    assert(newTab.rows_.size() >= 1);
    auto &tailHash = newTab.rows_[newTab.rows_.size() - 1].hash;
    auto tailBlob = store_.get(tailHash);
    auto tailRB = RowBank::deserialize(tailBlob);
    assert(!tailRB.isFull());
    tailRB.append(row);
    if (maybeNewRB) {
        newTab.rows_.push_back(maybeNewRB->hash());
    }
    return newTab;
}

DataSet::RowBanks Table::rows() const {
    return rows_;
}

