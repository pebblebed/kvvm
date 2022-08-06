#ifndef QUERY_incl_
#define QUERY_incl_

#include <unordered_set>
#include "table.hpp"

struct Query : public DataSet {

};

struct Slice : public Query {
  protected:
    IData &store_;
    const Schema schema_;
    const Table &parent_;
    const SliceDesc cols_;

  public:
    Slice(IData &store, const Table &parent, const SliceDesc &cols)
    : store_(store)
    , schema_(parent.schema().slice(cols))
    , parent_(parent)
    {}

    virtual HashedStruct flatten() const;
    virtual Schema schema() const;
    virtual RowBanks rows() const;
};

struct Filter: public Query {

};

struct Join: public Query {

};

#endif
