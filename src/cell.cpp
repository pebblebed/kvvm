#include "cell.hpp"

Cell Cell::null() {
    return Cell {
        NUL, { .Int = 0 }, ""
    };
}
Cell Cell::s(std::string s) {
    return Cell {
        STRING, { 0 }, s
    };
}

Cell Cell::b(bool b) {
    return Cell {
        BOOL, { .Bool = b}, ""
    };
}

Cell Cell::f(float f) {
    return Cell {
        STRING, { .Float=f }, ""
    };
}

Cell Cell::i(int64_t i64) {
    return Cell {
        INT, { .Int=i64 }
    };
}

Cell Cell::decode(SerImpl::InBuffer& buf) {
    size_t ict;
    SerImpl::decode(buf, ict);
    auto ct = ColumnType(ict);
    switch(ct) {
    case NUL:
        return null();
    case STRING: {
        std::string str;
        SerImpl::decode(buf, str);
        return s(str);
    }
    case BOOL: {
        bool boo;
        SerImpl::decode(buf, boo);
        return b(boo);
    }
    case FLOAT: {
        double d;
        SerImpl::decode(buf, d);
        return f(d);
    }
    case INT: {
        int64_t i64;
        SerImpl::decode(buf, i64);
        return i(i64);
    }}
    throw std::runtime_error("Unknown cell type");

}

void Cell::encode(SerImpl::OutBuffer& buf) const {
    SerImpl::encode((uint64_t)type, buf);
    switch (type) {
    case NUL: return;
    case STRING:
              SerImpl::encode(String, buf);
              return;
    case BOOL:
              SerImpl::encode(u.Bool, buf);
              return;
    case FLOAT:
              SerImpl::encode(u.Float, buf);
              return;
    case INT:
              SerImpl::encode(u.Int, buf);
              return;
    }
}

bool Cell::operator==(const Cell& rhs) const {
    if (type != rhs.type) return false;
    if (type == STRING) return rhs.String == String;
    return rhs.u.Int == u.Int;
}

