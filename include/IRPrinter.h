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

#ifndef BOOST_IRPRINTER_H
#define BOOST_IRPRINTER_H

#include <string>
#include <sstream>

#include "IRVisitor.h"


namespace Boost {

namespace Internal {

class IRPrinter : public IRVisitor {
 public:
    IRPrinter() : IRVisitor() {
        indent = 0;
        print_range = false;
        print_arg = false;
    }
    std::string print(const Expr&);
    std::string print(const Stmt&);
    std::string print(const Group&);

    void print_indent() {
        for (int i = 0; i < indent; ++i)
            oss << " ";
    }

    void enter() {
        indent += 2;
    }

    void exit() {
        indent -= 2;
    }

    void visit(Ref<const IntImm>) override;
    void visit(Ref<const UIntImm>) override;
    void visit(Ref<const FloatImm>) override;
    void visit(Ref<const StringImm>) override;
    void visit(Ref<const Unary>) override;
    void visit(Ref<const Binary>) override;
    void visit(Ref<const Select>) override;
    void visit(Ref<const Compare>) override;
    void visit(Ref<const Call>) override;
    void visit(Ref<const Var>) override;
    void visit(Ref<const Cast>) override;
    void visit(Ref<const Ramp>) override;
    void visit(Ref<const Index>) override;
    void visit(Ref<const Dom>) override;
    void visit(Ref<const LoopNest>) override;
    void visit(Ref<const IfThenElse>) override;
    void visit(Ref<const Move>) override;
    void visit(Ref<const Kernel>) override;
 private:
    std::ostringstream oss;
    int indent;
    bool print_range;
    bool print_arg;
};

}  // namespace Internal

}  // namespace Boost


#endif  // BOOST_IRPRINTER_H