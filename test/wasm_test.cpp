#include <gtest/gtest.h>
#include "../src/wasm_runtime.hpp"

TEST(Wasm, Exercise){
    auto runtime = WasmRuntime::load(std::string("123"));
    runtime->run(std::string("snark"));
    delete runtime;
}