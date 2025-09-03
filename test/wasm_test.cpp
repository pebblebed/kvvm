#include <gtest/gtest.h>
#include "../src/wasm_runtime.hpp"

std::string
load_hello_world() {
    FILE *fp = fopen(
        "../src/udfs/rust-hello-world/target/wasm32-wasip1/debug/hello_world.wasm",
        "r");
    assert(fp);
    fseek(fp, 0, SEEK_END);
    auto len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::string buf;
    buf.resize(len);
    fread(&buf[0], len, 1, fp);
    fclose(fp);
    return buf;
}

TEST(Wasm, Exercise){
    auto runtime = WasmRuntime::load(load_hello_world());
    runtime->run(std::string("snark"));
    delete runtime;
}