#include <fstream>
#include <string>


void clean_write(std::string dst, std::string src) {
    std::ofstream out(dst, std::ios::out);
    out << src;
    out.close();
}


int main() {
    clean_write("./kernels/kernel_example.cc",
        "#include \"../run.h\"\n\nvoid kernel_example(float (&B)[32][16], float (&C)[32][16], float (&A)[32][16]) {}");
    clean_write("./kernels/kernel_case1.cc",
        "#include \"../run.h\"\n\nvoid kernel_case1(float (&A)[32][16]) {}");
    clean_write("./kernels/kernel_case2.cc",
        "#include \"../run.h\"\n\nvoid kernel_case2(float (&A)[16][8]) {}");
    clean_write("./kernels/kernel_case3.cc",
        "#include \"../run.h\"\n\nvoid kernel_case3(int (&B)[16][32], int (&C)[16][32], int (&A)[16][32]) {}");
    clean_write("./kernels/kernel_case4.cc",
        "#include \"../run.h\"\n\nvoid kernel_case4(float (&B)[16][32], float (&C)[32][32], float (&A)[16][32]) {}");
    clean_write("./kernels/kernel_case5.cc",
        "#include \"../run.h\"\n\nvoid kernel_case5(float (&B)[16][32], float (&C)[32][32], float (&D)[16][32], float &alpha, float &beta, float (&A)[16][32]) {}");
    clean_write("./kernels/kernel_case6.cc",
        "#include \"../run.h\"\n\nvoid kernel_case6(float (&B)[2][16][7][7], float (&C)[8][16][3][3], float (&A)[2][8][5][5]) {}");
    clean_write("./kernels/kernel_case7.cc",
        "#include \"../run.h\"\n\nvoid kernel_case7(float (&A)[32][16], float (&B)[16][32]) {}");
    clean_write("./kernels/kernel_case8.cc",
        "#include \"../run.h\"\n\nvoid kernel_case8(float (&B)[8][16], float (&A)[8][2][16]) {}");
    clean_write("./kernels/kernel_case9.cc",
        "#include \"../run.h\"\n\nvoid kernel_case9(float (&B)[16][32][8], float (&C)[32][32], float (&D)[8][32], float (&A)[16][32]) {}");
    clean_write("./kernels/kernel_case10.cc",
        "#include \"../run.h\"\n\nvoid kernel_case10(float (&B)[10][10], float (&A)[8][8]) {}");
    return 0;
}