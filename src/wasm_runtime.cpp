#include "wasm_runtime.hpp"

#include <stdint.h>
#include <wasm_export.h>

struct WasmImpl: public WasmRuntime {
    char *buffer, error_buf[128];
    wasm_module_t module;
    wasm_module_inst_t module_inst;
    wasm_function_inst_t func;
    wasm_exec_env_t exec_env;
    const uint32_t stack_size = 8092, heap_size = 1024 * 1024;

    WasmImpl(const std::string& wasm_proggy) {
        wasm_runtime_init();
        auto progzo = (uint8_t*)&wasm_proggy[0];
        module = wasm_runtime_load(progzo, wasm_proggy.size(), error_buf, sizeof(error_buf));
    }

    virtual std::string run(std::string arg) {
        return std::string("snark");
    }
};

WasmRuntime*
WasmRuntime::load(const std::string& bytes) {
    return new WasmImpl(bytes);
}