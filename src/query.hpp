#ifndef QUERY_incl_
#define QUERY_incl_

#include <unordered_set>
#include "table.hpp"

struct Query : public DataSet {

};

struct Slice : public Query {
  protected:
    const Schema schema_;
    const SliceDesc cols_;

  public:
    Slice(const Table& parent, const SliceDesc& cols)
    : schema_(parent.schema().slice(cols))
    {}
};

struct Filter: public Query {

};

struct Join: public Query {

};

#endif
