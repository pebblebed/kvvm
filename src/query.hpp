#ifndef QUERY_incl_
#define QUERY_incl_

#include <unordered_set>
#include <functional>

#include "table.hpp"

#define FIRST_CLASS_SLICE
#ifdef FIRST_CLASS_SLICE
struct Slice : public DataSet {
  protected:
    const Schema schema_;
    const Table &parent_;
    const SliceDesc cols_;

  public:
    Slice(const Table &parent, const SliceDesc &cols)
    : schema_(parent.schema().slice(cols))
    , parent_(parent)
    {}

    virtual HashedStruct flatten() const;
    virtual Schema schema() const;
    virtual RowBanks rows() const;
};
#endif

struct Filter: public DataSet {
  protected:
    const Table& parent_;
    const std::function<bool(Row)> filterFn_;

  public:
    Filter(const Table& parent, std::function<bool(Row)> filterFn)
    : parent_(parent)
    , filterFn_(filterFn)
    { }

    virtual HashedStruct flatten() const;
    Schema schema() const {
        return parent_.schema();
    }

    virtual RowBanks rows() const;
};

struct Join: public DataSet {
  protected:
    const Table& left_;
    const Table& right_;
    const std::function<bool(Row, Row)> joinFn_;

  public:
    Join(const Table& left, const Table& right, std::function<bool(Row, Row)> joinFn)
    : left_(left)
    , right_(right)
    , joinFn_(joinFn)
    { }

    virtual HashedStruct flatten() const;
    Schema schema() const {
        return left_.schema().join(right_.schema());
    }

    virtual RowBanks rows() const;
};

#endif
