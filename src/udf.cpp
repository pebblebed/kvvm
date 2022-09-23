#include "udf.hpp"
#include "wasm_runtime.hpp"

Cell
MapUDF::apply(Cell arg) const {
    return arg;
}

Cell
ReduceUDF::apply(Cell soFar, std::vector<Cell> args) const {
    return soFar;
}

class ReduceUDFImpl: public ReduceUDF {
    WasmRuntime* rt_;

    ReduceUDFImpl(const std::string& bytes) {
        rt_ = WasmRuntime::load(bytes);
    }
};

ReduceUDF* load_reduce_udf(const std::string& udf_bytes) {

    return nullptr;
}