#include <string>
#include <iostream>

#include "IR.h"
#include "IRMutator.h"
#include "IRVisitor.h"
#include "IRPrinter.h"
#include "type.h"

using namespace Boost::Internal;

int main() {
    const int N = 256;
    const int C = 1024;
    const int P = 7;
    const int Q = 7;
    const int H = 9;
    const int W = 9;
    const int K = 1024;
    const int R = 3;
    const int S = 3;
    Type index_type = Type::int_scalar(32);
    Type data_type = Type::float_scalar(32);

    // index n
    Expr dom_n = Dom::make(index_type, 0, N);
    Expr n = Index::make(index_type, "n", dom_n, IndexType::Spatial);

    // index k
    Expr dom_k = Dom::make(index_type, 0, K);
    Expr k = Index::make(index_type, "k", dom_k, IndexType::Spatial);

    // index p
    Expr dom_p = Dom::make(index_type, 0, P);
    Expr p = Index::make(index_type, "p", dom_p, IndexType::Spatial);

    // index q
    Expr dom_q = Dom::make(index_type, 0, Q);
    Expr q = Index::make(index_type, "q", dom_q, IndexType::Spatial);

    // index c
    Expr dom_c = Dom::make(index_type, 0, C);
    Expr c = Index::make(index_type, "c", dom_c, IndexType::Reduce);

    // index h
    Expr dom_h = Dom::make(index_type, 0, H);
    Expr h = Index::make(index_type, "h", dom_h, IndexType::Spatial);

    // index w
    Expr dom_w = Dom::make(index_type, 0, W);
    Expr w = Index::make(index_type, "w", dom_w, IndexType::Spatial);

    // index r
    Expr dom_r = Dom::make(index_type, 0, R);
    Expr r = Index::make(index_type, "r", dom_r, IndexType::Reduce);

    // index s
    Expr dom_s = Dom::make(index_type, 0, S);
    Expr s = Index::make(index_type, "s", dom_s, IndexType::Reduce);

    // I
    Expr expr_I = Var::make(data_type, "I",
        {n, c, Binary::make(index_type, BinaryOpType::Add, p, r),
               Binary::make(index_type, BinaryOpType::Add, q, s)},
        {N, C, H, W});

    // W
    Expr expr_W = Var::make(data_type, "W", {k, c, r, s}, {K, C, R, S});

    // O
    Expr expr_O = Var::make(data_type, "O", {n, k, p, q}, {N, K, P, Q});

    // main stmt
    Stmt main_stmt = Move::make(
        expr_O,
        Binary::make(data_type, BinaryOpType::Add, expr_O,
            Binary::make(data_type, BinaryOpType::Mul, expr_I, expr_W)),
        MoveType::MemToMem
    );

    // loop nest
    Stmt loop_nest = LoopNest::make({n, k, p, q, c, r, s}, {main_stmt});

    // kernel
    Group kernel = Kernel::make("simple_conv2d", {expr_I, expr_W}, {expr_O}, {loop_nest}, KernelType::CPU);

    // visitor
    IRVisitor visitor;
    kernel.visit_group(&visitor);

    // mutator
    IRMutator mutator;
    kernel = mutator.mutate(kernel);

    // printer
    IRPrinter printer;
    std::string code = printer.print(kernel);

    std::cout << code;

    std::cout << "Success!\n";
    return 0;
}