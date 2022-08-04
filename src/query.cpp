#include "query.hpp"

// Blank for now

HashedStruct
Slice::flatten() const {
    HashedStruct hs;
    hs.magic = MAGIC__TABLE;
    for (auto r: rows()) {
        hs.hashen.push_back(r.hash);
    }
    return hs;
}

Schema
Slice::schema() const {
    return schema_;
}

DataSet::RowBanks
Slice::rows() const {
    std::vector<Hashable<RowBank>> ret;
    auto colNums = parent_.schema().sliceToColNums(cols_);

    std::function<Row(Row)> slizzo = [colNums](Row r) {
        Row retval {};
        for (auto i: colNums) {
            retval.cells.push_back(r.cells[i]);
        }
        return retval;
    };

    for (auto hprb: parent_.rows()) {
        // XXX: Cache and stuff!
        auto prb = hprb.materialize(store_);
        auto newRb = prb.apply(slizzo);

        ret.push_back(prb.apply(slizzo));
    }
    return ret;
}

