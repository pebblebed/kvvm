#ifndef QUERY_incl_
#define QUERY_incl_

#include "table.hpp"

struct Query : public DataSet {

};

struct Slice : public Query {
    
};

struct Filter: public Query {

};

struct Join: public Query {

};

#endif
