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

#ifndef BOOST_IRMUTATOR_H
#define BOOST_IRMUTATOR_H

#include "IR.h"


namespace Boost {

namespace Internal {

class IRMutator {
 public:
    Expr mutate(const Expr&);
    Stmt mutate(const Stmt&);
    Group mutate(const Group&);

    virtual Expr visit(Ref<const IntImm>);
    virtual Expr visit(Ref<const UIntImm>);
    virtual Expr visit(Ref<const FloatImm>);
    virtual Expr visit(Ref<const StringImm>);
    virtual Expr visit(Ref<const Unary>);
    virtual Expr visit(Ref<const Binary>);
    virtual Expr visit(Ref<const Select>);
    virtual Expr visit(Ref<const Compare>);
    virtual Expr visit(Ref<const Call>);
    virtual Expr visit(Ref<const Var>);
    virtual Expr visit(Ref<const Cast>);
    virtual Expr visit(Ref<const Ramp>);
    virtual Expr visit(Ref<const Index>);
    virtual Expr visit(Ref<const Dom>);
    virtual Stmt visit(Ref<const LoopNest>);
    virtual Stmt visit(Ref<const IfThenElse>);
    virtual Stmt visit(Ref<const Move>);
    virtual Group visit(Ref<const Kernel>);
 private:
};

}  // namespace Internal

}  // namespace Boost


#endif  // BOOST_IRMUTATOR_H