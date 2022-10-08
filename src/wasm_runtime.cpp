#include "wasm_runtime.hpp"

#include "string.h"
#include <stdexcept>
#include <stdint.h>
#include <wasm_export.h>

WasmRuntime::~WasmRuntime() {}


static void
wasm_fd_write(wasm_exec_env_t env, int fd, const char* input) {
    printf("WASM SEZ: %s to fd %d\n", input, fd);
}

static const char*
wasm_getenv(wasm_exec_env_t env, const char* name) {
    return getenv(name);
}

static void
wasm_getenv_sizes_get(wasm_exec_env_t env, size_t* environ_count, size_t* environ_buf_size) {
    *environ_count = 12;
    *environ_buf_size = 1024;
}

static void
wasm_proc_exit(wasm_exec_env_t env, int code) {
    printf("WASM SEZ: proc_exit(%d)\n", code);
}


struct WasmImpl: public WasmRuntime {
    char *buffer, error_buf[128];
    wasm_module_t module;
    wasm_module_inst_t module_inst;
    wasm_function_inst_t func;
    wasm_exec_env_t exec_env;
    const uint32_t stack_size = 8092, heap_size = 1024 * 1024;

    WasmImpl(const std::string& wasm_proggy) {
        static NativeSymbol native_symbols[] = {
            { "fd_write", (void*)wasm_fd_write, "(i$)" },
            { "environ_get", (void*)wasm_getenv, "($)$" },
            { "environ_sizes_get", (void*)wasm_getenv_sizes_get, "(I*I*)$" },
            { "proc_exit", (void*)wasm_proc_exit, "(i)" },
        };


        /* all the runtime memory allocations are retricted in the global_heap_buf array */
        static char global_heap_buf[512 * 1024];
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));

        /* configure the memory allocator for the runtime */
        init_args.mem_alloc_type = Alloc_With_Pool;
        init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
        init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

        /* configure the native functions being exported to WASM app */
        init_args.native_module_name = "env";
        init_args.n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);
        init_args.native_symbols = native_symbols;

        /* set maximum thread number if needed when multi-thread is enabled,
        the default value is 4 */
        init_args.max_thread_num = 4;

        /* initialize runtime environment with user configurations*/
        if (!wasm_runtime_full_init(&init_args)) {
            throw std::runtime_error("wasm_runtime_full_init failed.");
        }
        auto progzo = (uint8_t*)&wasm_proggy[0];
        module = wasm_runtime_load(progzo, wasm_proggy.size(), error_buf, sizeof(error_buf));
        module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));

    }

    virtual std::string run(std::string arg);
    virtual ~WasmImpl();
};


WasmRuntime*
WasmRuntime::load(const std::string& bytes) {
    return new WasmImpl(bytes);
}

std::string
WasmImpl::run(std::string arg) {
    auto func = wasm_runtime_lookup_function(module_inst, "main", NULL);
    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);

    // Allocate space in the runtime and record the buffer.
    auto sz = arg.size();
    uint32_t host_argv[2];
    host_argv[0] = wasm_runtime_module_dup_data(module_inst, &arg[0], sz);
    host_argv[1] = wasm_runtime_module_dup_data(module_inst, "main", 4);
    auto callee_argv = wasm_runtime_module_dup_data(module_inst, (const char*)host_argv, sizeof(host_argv));
    uint32_t guest_argv[] = { 2, callee_argv };

    // Call the function.
    wasm_runtime_call_wasm(exec_env, func, 2, guest_argv);    

    // XXX: fish out the return value?

    // Free the runtime memory.
    wasm_runtime_module_free(module_inst, host_argv[0]);
    wasm_runtime_module_free(module_inst, host_argv[1]);
    wasm_runtime_module_free(module_inst, callee_argv);

    return std::string();
}

WasmImpl::~WasmImpl() {
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
    wasm_runtime_destroy();
}
