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

#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"

namespace Boost {

namespace Internal {

Expr IntImm::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const IntImm>(shared_from_this()));
}


Expr UIntImm::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const UIntImm>(shared_from_this()));
}


Expr FloatImm::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const FloatImm>(shared_from_this()));
}


Expr StringImm::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const StringImm>(shared_from_this()));
}


Expr Unary::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Unary>(shared_from_this()));
}


Expr Binary::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Binary>(shared_from_this()));
}


Expr Compare::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Compare>(shared_from_this()));
}


Expr Select::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Select>(shared_from_this()));
}


Expr Call::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Call>(shared_from_this()));
}


Expr Cast::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Cast>(shared_from_this()));
}


Expr Ramp::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Ramp>(shared_from_this()));
}


Expr Var::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Var>(shared_from_this()));
}


Expr Dom::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Dom>(shared_from_this()));
}


Expr Index::mutate_expr(IRMutator *mutator) const {
    return mutator->visit(Ref<const Index>(shared_from_this()));
}


Stmt LoopNest::mutate_stmt(IRMutator *mutator) const {
    return mutator->visit(Ref<const LoopNest>(shared_from_this()));
}


Stmt IfThenElse::mutate_stmt(IRMutator *mutator) const {
    return mutator->visit(Ref<const IfThenElse>(shared_from_this()));
}


Stmt Move::mutate_stmt(IRMutator *mutator) const {
    return mutator->visit(Ref<const Move>(shared_from_this()));
}


Group Kernel::mutate_group(IRMutator *mutator) const {
    return mutator->visit(Ref<const Kernel>(shared_from_this()));
}

/**
 * IRVisitor
 */ 

void IntImm::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const IntImm>(shared_from_this()));
}


void UIntImm::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const UIntImm>(shared_from_this()));
}


void FloatImm::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const FloatImm>(shared_from_this()));
}


void StringImm::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const StringImm>(shared_from_this()));
}


void Unary::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Unary>(shared_from_this()));
}


void Binary::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Binary>(shared_from_this()));
}


void Compare::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Compare>(shared_from_this()));
}


void Select::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Select>(shared_from_this()));
}


void Call::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Call>(shared_from_this()));
}


void Cast::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Cast>(shared_from_this()));
}


void Ramp::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Ramp>(shared_from_this()));
}


void Var::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Var>(shared_from_this()));
}


void Dom::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Dom>(shared_from_this()));
}


void Index::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Index>(shared_from_this()));
}


void LoopNest::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const LoopNest>(shared_from_this()));
}


void IfThenElse::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const IfThenElse>(shared_from_this()));
}


void Move::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Move>(shared_from_this()));
}


void Kernel::visit_node(IRVisitor *visitor) const {
    return visitor->visit(Ref<const Kernel>(shared_from_this()));
}


}  // namespace Internal

}  // namespace Boost