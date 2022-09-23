#ifndef _UDF_HPP_
#define _UDF_HPP_

#include "cell.hpp"

// Since we're doing WASM copy in/out, intentionally use value types here.

class MapUDF {
    public:
        virtual Cell apply(const Cell arg) const = 0;
};
MapUDF* load_map_udf(const std::string& wasm_bytes);

class ReduceUDF {
    public:
        virtual Cell apply(const Cell soFar, const std::vector<Cell> args) const = 0;
};
ReduceUDF* load_reduce_udf(const std::string& wasm_bytes);

#endif