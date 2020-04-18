![Build](https://github.com/pku-compiler-design-spring/CompilerProject-2020Spring-Part1/workflows/C/C++%20CI/badge.svg?branch=master)

## Code Generation Compiler

This project is designed for undergraduate students who are taking Compiler Design courses in spring.

> Author: Size Zheng

> Email: zhengsz@pku.edu.cn

### BUG report And Bonus

__Format:__ [date] "message" by **reporters** [bonus]

1. [2020-4-14] "In run.cc case 4,5 golden array shape bug" by **Ye Yuan, Anjiang Wei, Yuyue Wang, Chenyang Yang** [+1]
2. [2020-4-15] "In document, input BNF bug in AList" by **Jing Mai, Can Su, Zixuan Ling** [+1]
3. [2020-4-16] "In CMakeLists.txt, link target to library" by **Chenqian Wang, Jiaqi Zhang, Wenqi Wang** [+1]

### 1. Overview
In this project, we provide several useful IR nodes and corresponding IRVisitor and IRMutator. The concept behind these structs are well studied in [Halide](https://github.com/halide/Halide) and [TVM](https://github.com/apache/incubator-tvm). Here we invent some new IR nodes and re-implement the Visitor and Mutator for them.

The purpose of this project is to help students to better understand how to build a IR system and implement a simple code generate tool.

The IR infrastructure of this project contains four levels:

```
Program
Group
Stmt
Expr
```
The first level `Program` is not explicitly implemented.
Each level of IR has several different type of nodes:
```
Group: Kernel
Stmt: LoopNest, IfThenElse, Move
Expr: IntImm,
      UIntImm,
      FloatImm,
      StringImm,
      Unary,
      Binary,
      Select,
      Compare,
      Call,
      Var,
      Cast,
      Ramp,
      Index,
      Dom
```

Use these IR nodes we can potentially represent many kinds of programs.

### 2. Build
```sh
mkdir build
cd build
cmake ..
make -j 4
```

### 3. Example
In `test` directory, thre are two examples of `gemm` and `conv2d`, they are good examples of how to represent computations by our IR infrastructure. If you run them:
```sh
cd build/test
./gemm
./conv2d
```
You can see the results are very similar to C programs, however, the printed strings are just intermediate representation, you can't run the printed strings for now. We hope you can improve current system to print exactly C/C++ programs and compile them using C/C++ compilers


### 4. Tasks
1. Please read the source code base throughly, you need to understand every parts of it.
2. You need to implment you C/C++ code genreation. Hints: learn how the IRPrinter works, imitate it and try to write a new IRVisitor which can print C/C++ source codes.
2. Go to `project1` directory, you will find many json files in `case` directory. The are inputs to your questions. For example, `example.json` contains:
```json
{
    "name": "kernel_example",
    "ins": ["B", "C"],
    "outs": ["A"],
    "data_type": "float",
    "kernel": "A<32, 16>[i, j] = C<32, 16>[i, j] * B<32, 16>[i, j];"
}
```
It means you need to generate a `.cc` file which implements the computation of `A<32, 16>[i, j] = C<32, 16>[i, j] * B<32, 16>[i, j];`. Put the computation in a function named `kernel_example`, whose inputs are `B` and `C`, and output is `A`, the data type is `float`. In the expression, we can see `A` has shape of [32, 16], and also `B` and `C`. So the function's signature is 
```c
void kernel_example(float (&B)[32][16], float (&C)[32][16], float (&A)[32][16])
```
Please try to generate C/C++ source files for these json files and put them under directory `kernels`.

4. Your code genration application source files should be placed in `solution` directory. (But your code genration passes can be put in outer directories such as `include` and `src`)


### 5. Notice
1. We present a silly solution in `solution` directory, please do not follow such silly manner. The example is just used to tell you how our framework works.
2. All the source files you put in `solution` directory should only contain one `main` function, as we will compile all the source files in `solution` directory into one executable file.
3. Please be careful and do not delete important files, which may break down the system.
4. If you want to test your designs, just enter the `build` directory, run `make -j 4`, you will see the binaries in `build` directory, there are sub-directories such as `project1`, your executable files should be placed there automatically.
5. You are not supposed to modify `run.h` and `run.cc`. These files will be changed to another version which contains the full 10 test cases, so any modification is meaningless.
6. If you are confused about what kinds of C/C++ code you are supposed to generate, see `solution/example_solution.cc`.

### 6. Judge
1. We provide auto-test file, after building the project, enter `build/project1`, and run `./test1`, you can see the results.
2. We only show you 6 cases and 4 cases are hidden. The TAs will test all the 10 files and decide scores according to how many cases you can pass. Don't be worried, the hidden cases are no more complex than the open cases. If you can handle the open cases, you should pass all hidden cases.
3. Do not copy the codes from others, we will do the check! Any intends to break this rule will result a 0 score to you.


### 7. How it works?
When you build the project, we will actually build four parts:
- the files in `include` and `src`
- the files in `test`
- the files in `project1` are compiled to one executable
- the files in `project1/solution` are compiled to one executable

And we will automatically clean files under `kernels/*.cc`, so you can't expect to modify them manually.

Then we will call the executable from `project1/solution` automatically, which is expected to generate all the functions and put them in `kernels/*.cc`.

At last, we will run `./test1` manually to see your results and decide your scores according to the results.
