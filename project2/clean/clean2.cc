#include <fstream>
#include <string>


void clean_write(std::string dst, std::string src) {
    std::ofstream out(dst, std::ios::out);
    out << src;
    out.close();
}


int main() {
    clean_write("./kernels/grad_case1.cc",
        "#include \"../run2.h\"\n\nvoid grad_case1(float (&B)[4][16], float (&dC)[4][16], float (&dA)[4][16]) {}");
    clean_write("./kernels/grad_case2.cc",
        "#include \"../run2.h\"\n\nvoid grad_case2(float (&A)[4][16], float (&dB)[4][16], float (&dA)[4][16]) {}");
    clean_write("./kernels/grad_case3.cc",
        "#include \"../run2.h\"\n\nvoid grad_case3(float (&B)[16][16], float (&dC)[4][16], float (&dA)[4][16]) {}");
    clean_write("./kernels/grad_case4.cc",
        "#include \"../run2.h\"\n\nvoid grad_case4(float (&B)[16][32], float (&C)[32][32], float (&dA)[16][32], float (&dB)[16][32], float (&dC)[32][32]) {}");
    clean_write("./kernels/grad_case5.cc",
        "#include \"../run2.h\"\n\nvoid grad_case5(float (&C)[32][32], float (&D)[4][32], float (&dA)[16][32], float (&dB)[16][32][4]) {}");
    clean_write("./kernels/grad_case6.cc",
        "#include \"../run2.h\"\n\nvoid grad_case6(float (&C)[8][16][3][3], float (&dA)[2][8][5][5], float (&dB)[2][16][7][7]) {}");
    clean_write("./kernels/grad_case7.cc",
        "#include \"../run2.h\"\n\nvoid grad_case7(float (&dB)[16][32], float (&dA)[32][16]) {}");
    clean_write("./kernels/grad_case8.cc",
        "#include \"../run2.h\"\n\nvoid grad_case8(float (&dB)[32], float (&dA)[2][16]) {}");
    clean_write("./kernels/grad_case9.cc",
        "#include \"../run2.h\"\n\nvoid grad_case9(float (&dB)[4][6], float (&dA)[4]) {}");
    clean_write("./kernels/grad_case10.cc",
        "#include \"../run2.h\"\n\nvoid grad_case10(float (&dA)[8][8], float (&dB)[10][8]) {}");
    return 0;
}