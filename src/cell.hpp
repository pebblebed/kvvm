#ifndef CELL_incl_
#define CELL_incl_

#include <string>
#include <vector>

#include "serialize.hpp"

enum ColumnType {
  NUL,
  STRING,
  BOOL,
  FLOAT,
  INT
};

typedef std::string ColumnName;
typedef std::pair<ColumnName, ColumnType> ColumnDesc;
typedef std::vector<ColumnName> SliceDesc;


struct Cell {
    const ColumnType type = NUL;
    typedef union {
        bool Bool;
        double Float;
        int64_t Int;
    } U;
    U u = { .Int = 0 };

    const std::string String;

    static Cell null();
    static Cell s(std::string s);
    static Cell b(bool b);
    static Cell f(float f);
    static Cell i(int64_t i64);
    static Cell decode(SerImpl::InBuffer& buf);

    void encode(SerImpl::OutBuffer& buf) const;

    bool operator==(const Cell& rhs) const;
};

struct Row {
    std::vector<Cell> cells;
};

namespace SerImpl {
static inline void
encode(const Cell& c, OutBuffer& buf) {
    c.encode(buf);
}

static inline void
decode(InBuffer& buf, Cell& c) {
    c.decode(buf);
}

static inline void
encode(const Row& r, SerImpl::OutBuffer& buf) {
    encode_vector(r.cells, buf);
}

static inline void
decode(SerImpl::InBuffer& buf, Row& r) {
    decode_vector(buf, r.cells);
}

}

#endif
