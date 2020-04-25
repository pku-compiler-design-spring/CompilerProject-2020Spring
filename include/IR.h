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

#ifndef BOOST_IR_H
#define BOOST_IR_H

#include <memory>
#include <string>

#include "type.h"
#include "arith.h"
#include "debug.h"

namespace Boost {

namespace Internal {

/**
 * This class is inspired by Halide IntrusivePtr
 * The difference is that we use std::shared_ptr
 */
template <typename T>
class Ref {

 protected:
    std::shared_ptr<T> ptr = nullptr;

 public:
    Ref() {}

    Ref(Ref<T> &other) : ptr(other.ptr) {}

    Ref(Ref<T> &&other) : ptr(std::move(other.ptr)) {}

    /**
     * allow constructing from sub-class
     */ 
    template<typename U, typename std::enable_if<std::is_base_of<T, U>::value>::type* = nullptr>
    Ref(Ref<U> &other) : ptr(other.real_ptr()) {}

    template<typename U, typename std::enable_if<std::is_base_of<T, U>::value>::type* = nullptr>
    Ref(Ref<U> &&other) : ptr(std::move(other.real_ptr())) {}

    /**
     * allow constructing from shared_ptr of sub-class
     */ 
    template<typename U, typename std::enable_if<std::is_base_of<T, U>::value>::type* = nullptr>
    Ref(std::shared_ptr<U> _ptr) : ptr(_ptr) {}

    bool defined() { return ptr != nullptr; }

    T *get() const { return ptr.get(); }

    /**
     * have to expose inner shared_ptr, required by constructor
     */ 
    void set_ptr(std::shared_ptr<T> other) {
        this->ptr = other;
    }

    std::shared_ptr<T> real_ptr() const {
        return this->ptr;
    }

    T &operator*() const { return *ptr; }

    T *operator->() const { return ptr.operator->(); }

    Ref<T> &operator=(Ref<T> &b) {
        this->ptr = b.ptr;
        return *this;
    }

    Ref<T> &operator=(Ref<T> &&b) {
        swap(this->ptr, b.ptr);
        return *this;
    }

    bool operator<(Ref<T> &b) const {
        /* Don't directly compare shared_ptr, for C++20 removes operator< */
        return this->get() < b.get();
    }
};

/**
 * different type of IRNodes
 */ 
enum class IRNodeType : short {
    // Groups
    Kernel,
    // Stmts
    LoopNest,
    IfThenElse,
    Move,
    // Exprs
    Unary,
    Binary,
    Select,
    Compare,
    Call,
    Var,
    Cast,
    Ramp,
    Index,
    IntImm,
    UIntImm,
    FloatImm,
    StringImm,
    Dom
};


/**
 * forward declaration
 */
class IRVisitor;
class IRMutator;
class Expr;
class Stmt;
class Group;


/**
 * this is the base class of all IR nodes
 */ 
class IRNode {
 public:
    IRNode(const IRNodeType _type) : _node_type(_type) {}

    IRNodeType node_type() const {
        return this->_node_type;
    }

    /**
     * for IRVisitor
     */ 
    virtual void visit_node(IRVisitor *visitor) const = 0;

 private:
    /**
     * indicate the concrete type of this IR node
     */ 
    IRNodeType _node_type;
};


/**
 * base node of expression
 */ 
class ExprNode : public IRNode {
 private:
    Type type_;
 public:
    ExprNode(Type _type, const IRNodeType node_type) : IRNode(node_type), type_(_type) {} 

    virtual ~ExprNode() = default;

    virtual Expr mutate_expr(IRMutator *mutator) const = 0;

    Type type() const {
        return type_;
    }
};


/**
 * base node of statement
 */ 
class StmtNode : public IRNode {
 private:

 public:
    StmtNode(IRNodeType _type) : IRNode(_type) {}

    virtual ~StmtNode() = default;

    virtual Stmt mutate_stmt(IRMutator *mutator) const = 0;
};


/**
 * base node of group
 */ 
class GroupNode : public IRNode {
 private:

 public:
    GroupNode(IRNodeType _type) : IRNode(_type) {}

    virtual ~GroupNode() = default;

    virtual Group mutate_group(IRMutator *mutator) const = 0;
};


/**
 * inherited from Halide
 */ 
class IntImm : public ExprNode, public std::enable_shared_from_this<IntImm> {
 private:
    int64_t value_;
 public:
    IntImm(Type _type, const int64_t _value) : ExprNode(_type, IRNodeType::IntImm), value_(_value)  {}

    /**
     * May need consider bits
     */
    int64_t value() const {
        return value_;
    }

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Ref<const IntImm> make(Type t, const int64_t _value) {
        return std::make_shared<const IntImm>(t, _value);
    }

    static const IRNodeType node_type_ = IRNodeType::IntImm;
};


/**
 * inherited from Halide
 */ 
class UIntImm : public ExprNode, public std::enable_shared_from_this<UIntImm> {
 private:
    uint64_t value_;
 public:
    UIntImm(Type _type, const uint64_t _value) : ExprNode(_type, IRNodeType::UIntImm), value_(_value) {}

    /**
     * May need consider bits
     */ 
    uint64_t value() const {
        return value_;
    }

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Ref<const UIntImm> make(Type t, const uint64_t _value) {
        return std::make_shared<const UIntImm>(t, _value);
    }

    static const IRNodeType node_type_ = IRNodeType::UIntImm;
};


/**
 * inherited from Halide
 */ 
class FloatImm : public ExprNode, public std::enable_shared_from_this<FloatImm> {
 private:
    double value_;
 public:
    FloatImm(Type _type, const double _value) : ExprNode(_type, IRNodeType::FloatImm), value_(_value) {}

    /**
     * May need consider bits
     */ 
    double value() const {
        return value_;
    }

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Ref<const FloatImm> make(Type t, const double _value) {
        return std::make_shared<const FloatImm>(t, _value);
    }

    static const IRNodeType node_type_ = IRNodeType::FloatImm;
};


/**
 * inherited from Halide
 */ 
class StringImm : public ExprNode, public std::enable_shared_from_this<StringImm> {
 private:
    std::string value_;
 public:
    StringImm(Type _type, const std::string _value) :
        ExprNode(_type, IRNodeType::StringImm), value_(_value) {}

    std::string value() const {
        return value_;
    }

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Ref<const StringImm> make(Type t, const std::string _value) {
        return std::make_shared<const StringImm>(t, _value);
    }

    static const IRNodeType node_type_ = IRNodeType::StringImm;
};


/**
 * a reference to expression
 */ 
class Expr : public Ref<const ExprNode> {
 public:
    Expr() : Ref<const ExprNode>() {}
    
    Expr(const Expr &other) : Ref<const ExprNode>(other.real_ptr()) {}

    Expr(const Expr &&other) : Ref<const ExprNode>(other.real_ptr()) {}

    template<typename U,
                typename std::enable_if<std::is_base_of<ExprNode, U>::value>::type* = nullptr>
    Expr(Ref<const U> &other) : Ref<const ExprNode>(other) {}

    template<typename U,
                typename std::enable_if<std::is_base_of<ExprNode, U>::value>::type* = nullptr>
    Expr(Ref<const U> &&other) : Ref<const ExprNode>(std::move(other)) {}

    template<typename U,
                typename std::enable_if<std::is_base_of<ExprNode, U>::value>::type* = nullptr>
    Expr(std::shared_ptr<const U> _ptr) : Ref<const ExprNode>(_ptr) {}

    /**
     * convenient constructors
     */ 
    explicit Expr(bool value) :
        Ref<const ExprNode>(UIntImm::make(Type::uint_scalar(1), static_cast<uint64_t>(value))) {}

    explicit Expr(uint8_t value) :
        Ref<const ExprNode>(UIntImm::make(Type::uint_scalar(8), static_cast<uint64_t>(value))) {}

    explicit Expr(uint16_t value) :
        Ref<const ExprNode>(UIntImm::make(Type::uint_scalar(16), static_cast<uint64_t>(value))) {}

    explicit Expr(uint32_t value) :
        Ref<const ExprNode>(UIntImm::make(Type::uint_scalar(32), static_cast<uint64_t>(value))) {}

    explicit Expr(uint64_t value) :
        Ref<const ExprNode>(UIntImm::make(Type::uint_scalar(64), value)) {}

    explicit Expr(int8_t value) :
        Ref<const ExprNode>(IntImm::make(Type::int_scalar(8), static_cast<int64_t>(value))) {}

    explicit Expr(int16_t value) :
        Ref<const ExprNode>(IntImm::make(Type::int_scalar(16), static_cast<int64_t>(value))) {}

    Expr(int value) :
        Ref<const ExprNode>(IntImm::make(Type::int_scalar(32), static_cast<int64_t>(value))) {}

    explicit Expr(int64_t value) :
        Ref<const ExprNode>(IntImm::make(Type::int_scalar(64), value)) {}

    explicit Expr(float value) :
        Ref<const ExprNode>(FloatImm::make(Type::float_scalar(32), static_cast<double>(value))) {}

    Expr(double value) :
        Ref<const ExprNode>(FloatImm::make(Type::float_scalar(64), value)) {}

    IRNodeType node_type() const {
        return this->get()->node_type();
    }

    Type type() const {
        return this->get()->type();
    }

    void visit_expr(IRVisitor *visitor) const {
        return this->get()->visit_node(visitor);
    }

    Expr mutate_expr(IRMutator *mutator) const {
        return this->get()->mutate_expr(mutator);
    }

    /**
     * cast to other type of reference
     */ 
    template <typename T>
    std::shared_ptr<const T> as() const {
        if (this->node_type() == T::node_type_) {
            return std::static_pointer_cast<const T>(this->real_ptr());
        }
        return nullptr;
    }
};


/**
 * a reference to statement
 */ 
class Stmt : public Ref<const StmtNode> {
 public:
    Stmt() : Ref<const StmtNode>() {}

    Stmt(const Stmt &other) : Ref<const StmtNode>(other.real_ptr()) {}

    Stmt(const Stmt &&other) : Ref<const StmtNode>(other.real_ptr()) {}

    template<typename U, typename std::enable_if<std::is_base_of<StmtNode, U>::value>::type* = nullptr>
    Stmt(Ref<const U> &other) : Ref<const StmtNode>(other) {}

    template<typename U, typename std::enable_if<std::is_base_of<StmtNode, U>::value>::type* = nullptr>
    Stmt(Ref<const U> &&other) : Ref<const StmtNode>(std::move(other)) {}

    template<typename U, typename std::enable_if<std::is_base_of<StmtNode, U>::value>::type* = nullptr>
    Stmt(std::shared_ptr<const U> _ptr) : Ref<const StmtNode>(_ptr) {}

    Stmt(std::shared_ptr<const StmtNode> _ptr) : Ref<const StmtNode>(_ptr) {}

    Stmt &operator=(const Stmt &other) {
        this->set_ptr(other.real_ptr());
        return *this;
    }

    IRNodeType node_type() const {
        return this->get()->node_type();
    }

    void visit_stmt(IRVisitor *visitor) const {
        return this->get()->visit_node(visitor);
    }

    Stmt mutate_stmt(IRMutator *mutator) const {
        return this->get()->mutate_stmt(mutator);
    }

    /**
     * cast to other type of reference
     */ 
    template <typename T>
    std::shared_ptr<const T> as() const {
        if (this->node_type() == T::node_type_) {
            return std::static_pointer_cast<const T>(this->real_ptr());
        }
        return nullptr;
    }
};


/**
 * a reference to group
 */ 
class Group : public Ref<const GroupNode> {
 public:
    Group() : Ref<const GroupNode>() {}

    Group(const Group &other) : Ref<const GroupNode>(other.real_ptr()) {}

    Group(const Group &&other) : Ref<const GroupNode>(other.real_ptr()) {}

    template<typename U, typename std::enable_if<std::is_base_of<GroupNode, U>::value>::type* = nullptr>
    Group(Ref<const U> &other) : Ref<const GroupNode>(other) {}

    template<typename U, typename std::enable_if<std::is_base_of<GroupNode, U>::value>::type* = nullptr>
    Group(Ref<const U> &&other) : Ref<const GroupNode>(std::move(other)) {}

    template<typename U, typename std::enable_if<std::is_base_of<GroupNode, U>::value>::type* = nullptr>
    Group(std::shared_ptr<const U> _ptr) : Ref<const GroupNode>(_ptr) {}

    Group(std::shared_ptr<const GroupNode> _ptr) : Ref<const GroupNode>(_ptr) {}

    Group &operator=(const Group &other) {
        this->set_ptr(other.real_ptr());
        return *this;
    }

    IRNodeType node_type() const {
        return this->get()->node_type();
    }

    void visit_group(IRVisitor *visitor) const {
        return this->get()->visit_node(visitor);
    }

    Group mutate_group(IRMutator *mutator) const {
        return this->get()->mutate_group(mutator);
    }

    /**
     * cast to other type of reference
     */ 
    template <typename T>
    std::shared_ptr<const T> as() const {
        if (this->node_type() == T::node_type_) {
            return std::static_pointer_cast<const T>(this->real_ptr());
        }
        return nullptr;
    }
};


enum class UnaryOpType : uint8_t {
    Neg,    /* negate */
    Not     /* logic not */
};


/**
 * unary operation
 */ 
class Unary : public ExprNode, public std::enable_shared_from_this<Unary> {
 public:
    UnaryOpType op_type;
    Expr a;

    Unary(Type _type, UnaryOpType _op_type, Expr _a) : ExprNode(_type, IRNodeType::Unary),
        op_type(_op_type), a(_a) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, UnaryOpType _op_type, Expr _a) {
        return std::make_shared<const Unary>(t, _op_type, _a);
    }

    static const IRNodeType node_type_ = IRNodeType::Unary;
};


enum class BinaryOpType : uint8_t {
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    And,
    Or,
};


/**
 * binary operation
 */ 
class Binary : public ExprNode, public std::enable_shared_from_this<Binary> {
 public:
    BinaryOpType op_type;
    Expr a, b;

    Binary(Type _type, BinaryOpType _op_type, Expr _a, Expr _b) : ExprNode(_type, IRNodeType::Binary),
        op_type(_op_type), a(_a), b(_b) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, BinaryOpType _op_type, Expr _a, Expr _b) {
        return std::make_shared<const Binary>(t, _op_type, _a, _b);
    }

    static const IRNodeType node_type_ = IRNodeType::Binary;
};


enum class CompareOpType : uint8_t {
    LT,
    LE,
    EQ,
    NE,
    GE,
    GT
};


/**
 * compare op <, <=, =, !=, >=, >
 */ 
class Compare : public ExprNode, public std::enable_shared_from_this<Compare> {
 public:
    CompareOpType op_type;
    Expr a, b;

    Compare(Type _type, CompareOpType _op_type, Expr _a, Expr _b) : ExprNode(_type, IRNodeType::Compare),
        op_type(_op_type), a(_a), b(_b) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, CompareOpType _op_type, Expr _a, Expr _b) {
        return std::make_shared<const Compare>(t, _op_type, _a, _b);
    }

    static const IRNodeType node_type_ = IRNodeType::Compare;
};


/**
 * select op: cond? true_value : false_value
 */ 
class Select : public ExprNode, public std::enable_shared_from_this<Select> {
 public:
    Expr cond;
    Expr true_value, false_value;

    Select(Type _type, Expr _cond, Expr _true_value, Expr _false_value) : ExprNode(_type, IRNodeType::Select),
        cond(_cond), true_value(_true_value), false_value(_false_value) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, Expr _cond, Expr _true_value, Expr _false_value) {
        return std::make_shared<const Select>(t, _cond, _true_value, _false_value);
    }

    static const IRNodeType node_type_ = IRNodeType::Select;
};


enum class CallType : uint8_t {
    Pure,
    SideEffect
};


/**
 * call op, used for function call
 */ 
class Call : public ExprNode, public std::enable_shared_from_this<Call> {
 public:
    std::vector<Expr> args;
    std::string func_name;
    CallType call_type;

    Call(Type _type, const std::vector<Expr> &_args, const std::string &_func_name, CallType _call_type) : ExprNode(_type, IRNodeType::Call),
        args(_args), func_name(_func_name), call_type(_call_type) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;
    
    static Expr make(Type t, const std::vector<Expr> &_args, const std::string &_func_name, CallType _call_type) {
        return std::make_shared<const Call>(t, _args, _func_name, _call_type);
    }

    static const IRNodeType node_type_ = IRNodeType::Call;
};


/**
 * cast op, used for type cast
 */ 
class Cast : public ExprNode, public std::enable_shared_from_this<Cast> {
 public:
    Type new_type;
    Expr val;

    Cast(Type _type, Type _new_type, Expr _val) : ExprNode(_type, IRNodeType::Cast),
        new_type(_new_type), val(_val) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, Type _new_type, Expr _val) {
        return std::make_shared<const Cast>(t, _new_type, _val);
    }

    static const IRNodeType node_type_ = IRNodeType::Cast;
};


/**
 * ramp, used for vectorization
 * - broadcast: when stride is 0
 */ 
class Ramp : public ExprNode, public std::enable_shared_from_this<Ramp> {
 public:
    Expr base;
    uint16_t stride;
    uint16_t lanes;

    Ramp(Type _type, Expr _base, uint16_t _stride, uint16_t _lanes) : ExprNode(_type, IRNodeType::Ramp),
        base(_base), stride(_stride), lanes(_lanes) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, Expr _base, uint16_t _stride, uint16_t _lanes) {
        return std::make_shared<const Ramp>(t, _base, _stride, _lanes);
    }

    static const IRNodeType node_type_ = IRNodeType::Ramp;
};


/**
 * TODO: do we need shuffle?
 */ 


/**
 * variable index expression, such as A[i, j]
 * - scalar: when shape is {1}
 */ 
class Var : public ExprNode, public std::enable_shared_from_this<Var> {
 public:
    std::string name;
    std::vector<Expr> args;
    // TODO: this may need to be removed to other class
    std::vector<size_t> shape;

    Var(Type _type, const std::string &_name, const std::vector<Expr> &_args,
        const std::vector<size_t> &_shape) : ExprNode(_type, IRNodeType::Var),
        name(_name), args(_args), shape(_shape) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, const std::string &_name, const std::vector<Expr> &_args,
        const std::vector<size_t> &_shape) {
        return std::make_shared<const Var>(t, _name, _args, _shape);
    }

    static const IRNodeType node_type_ = IRNodeType::Var;
};


/**
 * iteration domain, for now it's a simple [begin, begin+extent)
 */ 
class Dom : public ExprNode, public std::enable_shared_from_this<Dom> {
 public:
    Expr begin;
    Expr extent;

    Dom(Type _type, Expr _begin, Expr _extent) : ExprNode(_type, IRNodeType::Dom), begin(_begin), extent(_extent) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;
    
    static Expr make(Type t, Expr _begin, Expr _extent) {
        return std::make_shared<const Dom>(t, _begin, _extent);
    }

    static const IRNodeType node_type_ = IRNodeType::Dom;
};


enum class IndexType : uint8_t {
    Spatial,
    Reduce,
    Thread,
    Block,
    Vectorized,
    Unrolled
};


/**
 * iteration index
 */ 
class Index : public ExprNode, public std::enable_shared_from_this<Index> {
 public:
    std::string name;
    Expr dom;
    IndexType index_type;

    Index(Type _type, const std::string &_name, Expr _dom, IndexType _index_type) :
        ExprNode(_type, IRNodeType::Index), name(_name), dom(_dom), index_type(_index_type) {}

    Expr mutate_expr(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Expr make(Type t, const std::string &_name, Expr _dom, IndexType _index_type) {
        return std::make_shared<const Index>(t, _name, _dom, _index_type);
    }

    static const IRNodeType node_type_ = IRNodeType::Index;
};


/**
 * loop nest
 * - block: if index_list is empty, it means a block of statements
 */ 
class LoopNest : public StmtNode, public std::enable_shared_from_this<LoopNest> {
 public:
    std::vector<Expr> index_list;
    std::vector<Stmt> body_list;

    LoopNest(const std::vector<Expr> &_index_list, const std::vector<Stmt> &_body_list) :
        StmtNode(IRNodeType::LoopNest), index_list(_index_list), body_list(_body_list) {}

    Stmt mutate_stmt(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;

    static Stmt make(const std::vector<Expr> &_index_list, const std::vector<Stmt> &_body_list) {
        return std::make_shared<const LoopNest>(_index_list, _body_list);
    }

    static const IRNodeType node_type_ = IRNodeType::LoopNest;
};


/**
 * branch statement
 */ 
class IfThenElse : public StmtNode, public std::enable_shared_from_this<IfThenElse> {
 public:
    Expr cond;
    Stmt true_case;
    Stmt false_case;

    IfThenElse(Expr _cond, Stmt _true_case, Stmt _false_case) :
        StmtNode(IRNodeType::IfThenElse), cond(_cond), true_case(_true_case), false_case(_false_case) {}

    Stmt mutate_stmt(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;
    
    static Stmt make(Expr _cond, Stmt _true_case, Stmt _false_case) {
        return std::make_shared<const IfThenElse>(_cond, _true_case, _false_case);
    }

    static const IRNodeType node_type_ = IRNodeType::IfThenElse;
};


enum class MoveType : uint8_t {
    DeviceToHost,
    HostToDevice,
    MemToShared,
    SharedToMem,
    MemToLocal,
    LocalToMem,
    SharedToLocal,
    LocalToShared,
    SharedToShared,
    MemToMem,
    LocalToLocal
};


/**
 * assign statement: load and store are put together
 * - evaluate: when dst is nullptr
 */ 
class Move : public StmtNode, public std::enable_shared_from_this<Move> {
 public:
    Expr dst;
    Expr src;
    // TODO: is this necessary?
    MoveType move_type;

    Move(Expr _dst, Expr _src, MoveType _move_type) :
        StmtNode(IRNodeType::Move), dst(_dst), src(_src), move_type(_move_type) {}

    Stmt mutate_stmt(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;
    
    static Stmt make(Expr _dst, Expr _src, MoveType _move_type) {
        return std::make_shared<const Move>(_dst, _src, _move_type);
    }

    static const IRNodeType node_type_ = IRNodeType::Move;
};


enum class KernelType : uint8_t {
    CPU,
    GPU
};


class Kernel : public GroupNode, public std::enable_shared_from_this<Kernel> {
 public:
    std::string name;
    std::vector<Expr> inputs;
    std::vector<Expr> outputs;
    std::vector<Stmt> stmt_list;
    KernelType kernel_type;

    Kernel(const std::string &_name, const std::vector<Expr> &_inputs,
        const std::vector<Expr> &_outputs, const std::vector<Stmt> &_stmt_list, KernelType _kernel_type) :
        GroupNode(IRNodeType::Kernel), name(_name), inputs(_inputs), outputs(_outputs),
        stmt_list(_stmt_list), kernel_type(_kernel_type) {}

    Group mutate_group(IRMutator *mutator) const;
    void visit_node(IRVisitor *visitor) const;
    
    static Group make(const std::string &_name, const std::vector<Expr> &_inputs,
        const std::vector<Expr> &_outputs, const std::vector<Stmt> &_stmt_list, KernelType _kernel_type) {
        return std::make_shared<const Kernel>(_name, _inputs, _outputs, _stmt_list, _kernel_type);
    }

    static const IRNodeType node_type_ = IRNodeType::Kernel;
};


}  // namespace Internal

}  // namespace Boost


#endif  // BOOST_IR_H