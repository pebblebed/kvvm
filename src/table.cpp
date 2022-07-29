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

static const int RowBankMax = 512;

bool
isFull(const RowBank& rb) {
    return rb.size() == RowBankMax;
}

Table Table::addRow(const Row& row) {
    RowBank* maybeNewRB = nullptr;
    if (rows_.size() == 0) {
        dbg(DebugClass::table, 1, "allocating first RB!\n");
        maybeNewRB = new RowBank();
    } else {
        auto tailHash = rows_[rows_.size() - 1].hash;
        auto tailBlob = store_.get(tailHash);
        auto tailRB = RowBank::deserialize(tailBlob);
        if (isFull(tailRB)) {
            dbg(DebugClass::table, 1, "allocating %d'th RB!\n",
                int(rows_.size()));
            maybeNewRB = new RowBank();
        }
    }
    if (maybeNewRB) {
        dbg(DebugClass::table, 1, "new RB!");
        auto appendedRows = rows_;
        appendedRows.push_back(*maybeNewRB);
        Table newTab(store_, schema_, appendedRows);
        // Just get the happy-path body inlined recursively
        auto retval = newTab.addRow(row);
        delete maybeNewRB;
        return retval;
    }
    assert(!maybeNewRB);
    Table newTab(store_, schema_, rows_);
    // OK! Preliminaries out of the way: we have space allocated for the new row.
    assert(newTab.rows_.size() >= 1);
    auto &tailHash = newTab.rows_[newTab.rows_.size() - 1].hash;
    auto tailBlob = store_.get(tailHash);
    auto tailRB = RowBank::deserialize(tailBlob);
    assert(!isFull(tailRB));
    tailRB.append(row);
    return newTab;
}

DataSet::RowBanks Table::rows() const {
    return rows_;
}

