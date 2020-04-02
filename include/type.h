/*
 * MIT License
 * 
 * Copyright (c) 2020 Size Zheng

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef BOOST_TYPE_H
#define BOOST_TYPE_H

#include <vector>
#include <sstream>

#include "debug.h"


namespace Boost {

namespace Internal {

enum class TypeCode : uint8_t {
    Int,
    UInt,
    Float,
    String,
    Handle
};

/**
 * This maybe a heavy implementation
 */ 
class LanesList {
 private:
    std::vector<uint16_t> lanes_list;
 public:
    LanesList() {}

    LanesList(const LanesList &other) : lanes_list(other.lanes_list) {}

    LanesList(const LanesList &&other) : lanes_list(std::move(other.lanes_list)) {}

    LanesList(const std::vector<uint16_t> &_lanes_list) : lanes_list(_lanes_list) {};

    LanesList(const std::vector<uint16_t> &&_lanes_list) {
        lanes_list = std::move(_lanes_list);
    }

    LanesList &push_back(uint16_t v) {
        lanes_list.push_back(v);
        return *this;
    }

    uint16_t pop_back() {
        uint16_t ret = lanes_list.back();
        lanes_list.pop_back();
        return ret;
    }

    size_t size() {
        return lanes_list.size();
    }

    uint16_t &operator[](size_t pos) {
        return lanes_list[pos];
    }

    LanesList &operator=(const LanesList &other) {
        this->lanes_list = other.lanes_list;
        return *this;
    }

    LanesList &operator=(const LanesList &&other) {
        this->lanes_list = std::move(other.lanes_list);
        return *this;
    }

    bool operator==(const LanesList &other) const {
        if (this->lanes_list.size() != other.lanes_list.size())
            return false;
        for (size_t i = 0; i < lanes_list.size(); ++i) {
            if (this->lanes_list[i] != other.lanes_list[i]) {
                return false;
            }
        }
        return true;
    }

    friend std::ostream &operator<<(std::ostream& out, const LanesList &l) {
        out << "<";
        for (size_t i = 0; i < l.lanes_list.size(); ++i) {
            if (i == l.lanes_list.size() - 1) {
                out << l.lanes_list[i];
            } else {
                out << l.lanes_list[i] << ", ";
            }
        }
        out << ">";
        return out;
    }

    bool operator!=(const LanesList &other) const {
        return !((*this) == other);
    }
};


class Type {
 public:
    TypeCode code;
    uint16_t bits;
    LanesList lanes_list;

    Type() : lanes_list() {};

    Type(TypeCode _code, uint16_t _bits, LanesList _lanes_list) : code(_code),
        bits(_bits), lanes_list(_lanes_list) {}

    Type(Type &other) : code(other.code), bits(other.bits), lanes_list(other.lanes_list) {}

    Type(Type &&other) : code(other.code), bits(other.bits), lanes_list(std::move(other.lanes_list)) {}

    Type(const Type &other) : code(other.code), bits(other.bits), lanes_list(other.lanes_list) {}

    Type(const Type &&other) : code(other.code), bits(other.bits), lanes_list(std::move(other.lanes_list)) {}

    Type &operator=(const Type &other) {
        this->code = other.code;
        this->bits = other.bits;
        this->lanes_list = other.lanes_list;
        return *this;
    }

    Type &operator=(const Type &&other) {
        this->code = other.code;
        this->bits = other.bits;
        this->lanes_list = std::move(other.lanes_list);
        return *this;
    }

    bool is_int() const {
        return this->code == TypeCode::Int;
    }

    bool is_uint() const {
        return this->code == TypeCode::UInt;
    }

    bool is_float() const {
        return this->code == TypeCode::Float;
    }

    bool operator==(const Type &other) const {
        return this->code == other.code && this->bits == other.bits && this->lanes_list == other.lanes_list;
    }

    bool operator!=(const Type &other) const {
        return !((*this) == other);
    }

    friend std::ostream &operator<<(std::ostream& out, const Type &t) {
        out << "(";
        if (t.code == TypeCode::Int) {
            out << "int";
        } else if (t.code == TypeCode::UInt) {
            out << "uint";
        } else if (t.code == TypeCode::Float) {
            out << "float";
        } else if (t.code == TypeCode::String) {
            out << "string";
        } else if (t.code == TypeCode::Handle) {
            out << "handle";
        }
        out << t.bits << "_t ";
        out << t.lanes_list << ")";
        return out;
    }

    size_t dim() {
        return lanes_list.size();
    }

    static Type int_scalar(int bits) {
        CHECK(bits > 0 && bits < INT16_MAX, "bits too large: %d", bits);
        return Type(
            TypeCode::Int,
            static_cast<uint16_t>(bits),
            LanesList({static_cast<uint16_t>(1)}));
    }

    static Type uint_scalar(int bits) {
        CHECK(bits > 0 && bits < INT16_MAX, "bits too large: %d", bits);
        return Type(
            TypeCode::UInt,
            static_cast<uint16_t>(bits),
            LanesList({static_cast<uint16_t>(1)}));
    }

    static Type float_scalar(int bits) {
        CHECK(bits > 0 && bits < INT16_MAX, "bits too large: %d", bits);
        return Type(
            TypeCode::Float,
            static_cast<uint16_t>(bits),
            LanesList({static_cast<uint16_t>(1)}));
    }
};

}  // namespace Internal

}  // namespace Boost

#endif  // BOOST_TYPE_H