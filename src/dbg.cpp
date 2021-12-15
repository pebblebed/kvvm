#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cstdio>

#include "dbg.hpp"

using namespace std;

int debug_levels[NumDebugClasses];

namespace impl {

struct Initializer {
    Initializer() {
        auto cp = std::getenv("DBG");
        if (!cp) return;
        for (auto start = cp; *start; ) {
            auto eqp = strchr(start, ':');
            if (!eqp) break;
            auto level = atoi(eqp + 1);
            size_t tokLen = eqp - start;
#define DEBUG_CLASS(name) \
            if (0 == strncmp(#name, start, std::min(strlen(#name), tokLen))){ \
                debug_levels[name] = level; \
                dbg(_dbg, 1, "DBG(" #name") = %d\n", level); \
            }
            DEBUG_CLASSES
#undef DEBUG_CLASS
            auto nextp = strchr(start, ',');
            if (!nextp) break;
            start = nextp + 1;
        }
        dbg(_dbg, 1, "DBG initialized!\n");
    }
};

static Initializer boo;

void dbgImpl(const char* fmt, va_list args) {
    vprintf(fmt, args);
}

}

void
dbg(DebugClass cls, int level, const char* fmt, ...) {
    if (debug_levels[cls] >= level) {
        va_list args;
        va_start(args, fmt);
        impl::dbgImpl(fmt, args);
        va_end(args);
    }    
}
