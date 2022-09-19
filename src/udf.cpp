#include "udf.hpp"

Cell
MapUDF::apply(Cell arg) const {
    return arg;
}

Cell
ReduceUDF::apply(Cell soFar, std::vector<Cell> args) const {
    return soFar;
}
