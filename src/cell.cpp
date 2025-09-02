#include <stdexcept>
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
    uint8_t ict;
    SerImpl::decode(buf, ict);
    auto ct = ColumnType(ict);
    dbg(ser, 5, "Cell::decode ...");
    switch(ct) {
    case NUL:
        return null();
    case STRING: {
        std::string str;
        SerImpl::decode(buf, str);
        dbg(ser, 5, "string \"%s\"\n", str.c_str());
        return s(str);
    }
    case BOOL: {
        bool boo;
        SerImpl::decode(buf, boo);
        dbg(ser, 5, "bool %c\n", boo ? 'T' : 'F');
        return b(boo);
    }
    case FLOAT: {
        double d;
        SerImpl::decode(buf, d);
        dbg(ser, 5, "float %g\n", d);
        return f(d);
    }
    case INT: {
        int64_t i64;
        SerImpl::decode(buf, i64);
        dbg(ser, 5, "int %llx(==%lld)\n", i64, i64);
        return i(i64);
    }}
    throw std::runtime_error("Unknown cell type");

}

void Cell::encode(SerImpl::OutBuffer& buf) const {
    dbg(ser, 5, "Cell::encode ...");
    assert(int8_t(type) >= 0);
    assert(int8_t(type) <= INT8_MAX);
    SerImpl::encode((uint8_t)type, buf);
    switch (type) {
    case NUL: return;
    case STRING:
              dbg(ser, 5, "string \"%s\"\n", String.c_str());
              SerImpl::encode(String, buf);
              return;
    case BOOL:
              dbg(ser, 5, "bool %c\n", u.Bool ? 'T' : 'F');
              SerImpl::encode(u.Bool, buf);
              return;
    case FLOAT:
              dbg(ser, 5, "float %g\n", u.Float);
              SerImpl::encode(u.Float, buf);
              return;
    case INT:
              dbg(ser, 5, "int %llx(==%lld)\n", u.Int, u.Int);
              SerImpl::encode(u.Int, buf);
              return;
    }
}

bool Cell::operator==(const Cell& rhs) const {
    if (type != rhs.type) return false;
    if (type == STRING) return rhs.String == String;
    return rhs.u.Int == u.Int;
}

