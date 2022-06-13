#ifndef DBG_incl_
#define DBG_incl_

#include <cstdint>
#include <cstdio>
#include <cstdarg>

#define DEBUG_CLASSES \
    DEBUG_CLASS(_dbg) \
    DEBUG_CLASS(blob) \
    DEBUG_CLASS(ser) \
    DEBUG_CLASS(table) 

enum DebugClass {
#define DEBUG_CLASS(name) \
    name,
    DEBUG_CLASSES
#undef DEBUG_CLASS
    NumDebugClasses
};

extern int debug_levels[NumDebugClasses];

#ifdef NDEBUG
#define dbg(...) do {} while(0)
#else
extern void dbg(DebugClass cls, int level, const char* fmt, ...);

#endif

#endif