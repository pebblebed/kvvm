#ifndef _WASM_RUNTIME_HPP_
#define _WASM_RUNTIME_HPP_

#include <string>

struct WasmRuntime {
  static WasmRuntime* load(const std::string& wasm_bytes);
  virtual std::string run(std::string arg1) = 0;
  virtual ~WasmRuntime();
};

#endif
