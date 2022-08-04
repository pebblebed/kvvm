#include <string>
#include <iostream>

#include "table.hpp"
#include "serialize.hpp"

using namespace std;

Schema
Table::schema() const {
    return schema_;
}

HashedStruct Table::flatten() const {
  HashedStruct hs;
  hs.magic = MAGIC__TABLE;
  hs.hashen.push_back(schema_.hash());
  for (auto r: rows_) {
    hs.hashen.push_back(r.hash);
  }
  return hs;
}

Table Table::addCol(string name, Cell defaultVal) {
  auto prevCols = schema_.getCols();
  prevCols.push_back(make_pair(name, defaultVal.type));
  return Table(store_, Schema(prevCols));
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
    auto newRows = rows_;
    // OK! Preliminaries out of the way: we have space allocated for the new row.
    assert(newRows.size() >= 1);
    Hash &tailHash = newRows[newRows.size() - 1].hash;
    auto tailBlob = store_.get(tailHash);
    auto tailRB = RowBank::deserialize(tailBlob);
    assert(!isFull(tailRB));
    tailRB.append(row);
    // This is where new rows "come from", at least from the storage
    // system's perspective.
    tailRB.christen(store_);
    tailHash = tailRB.hash();
    return Table(store_, schema_, newRows);
}

DataSet::RowBanks Table::rows() const {
    return rows_;
}

