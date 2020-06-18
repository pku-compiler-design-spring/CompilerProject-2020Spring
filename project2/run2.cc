#include <iostream>
#include <random>

#include "run2.h"

const double eps1 = 1e-3;
const double eps2 = 1e-4;


bool test_case1(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "C<4, 16>[i, j] = A<4, 16>[i, j] * B<4, 16>[i, j] + 1.0;"
    // "dA<4, 16>[i, j] = dC<4, 16>[i, j] * B<4, 16>[i, j];"
    float B[4][16] = {{0}};
    float dA[4][16] = {{0}};
    float dC[4][16] = {{0}};
    float golden[4][16] = {{0}};
    // initialize
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            B[i][j] = dis(gen);
            dC[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            golden[i][j] = dC[i][j] * B[i][j];
        }
    }
    try {
        grad_case1(B, dC, dA);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            if (std::abs(golden[i][j] - dA[i][j]) >= eps1 && std::abs((golden[i][j] - dA[i][j]) / golden[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


bool test_case2(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "B<4, 16>[i, j] = A<4, 16>[i, j] * A<4, 16>[i, j] + 1.0;"
    // "dA<4, 16>[i, j] = dB<4, 16>[i, j] * A<4, 16>[i, j] + A<4, 16>[i, j] * dB<4, 16>[i, j];"
    float dB[4][16] = {{0}};
    float A[4][16] = {{0}};
    float dA[4][16] = {{0}};
    float golden[4][16] = {{0}};
    // initialize
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            dB[i][j] = dis(gen);
            A[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            golden[i][j] = 2 * dB[i][j] * A[i][j];
        }
    }
    try {
        grad_case2(A, dB, dA);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            if (std::abs(golden[i][j] - dA[i][j]) >= eps1 && std::abs((golden[i][j] - dA[i][j]) / golden[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


bool test_case3(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "C<4, 16>[i, j] = A<4, 16>[i, k] * B<16, 16>[k, j];"
    // "dA<4, 16>[i, k] = dC<4, 16>[i, j] * B<16, 16>[k, j];"
    float B[16][16] = {{0}};
    float dC[4][16] = {{0}};
    float dA[4][16] = {{0}};
    float golden[4][16] = {{0}};
    // initialize
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            B[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            dC[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            for (int k = 0; k < 16; ++k) {
                golden[i][k] += (dC[i][j] * B[k][j]);
            }
        }
    }
    try {
        grad_case3(B, dC, dA);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 16; ++j) {
            if (std::abs(golden[i][j] - dA[i][j]) >= eps1 && std::abs((golden[i][j] - dA[i][j]) / golden[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


bool test_case4(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "A<16, 32>[i, j] = B<16, 32>[i, k] * C<32, 32>[k, j];"
    // "dB<16, 32>[i, k] = dA<16, 32>[i, j] * C<32, 32>[k, j];"
    // "dC<32, 32>[k, j] = dA<16, 32>[i, j] * B<16, 32>[i, k];"
    float B[16][32] = {{0}};
    float C[32][32] = {{0}};
    float dA[16][32] = {{0}};
    float dB[16][32] = {{0}};
    float dC[32][32] = {{0}};
    float goldenB[16][32] = {{0}};
    float goldenC[32][32] = {{0}};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            B[i][j] = dis(gen);
            dA[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            C[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            for (int k = 0; k < 32; ++k) {
                goldenB[i][k] += dA[i][j] * C[k][j];
                goldenC[k][j] += dA[i][j] * B[i][k];
            }
        }
    }
    try {
        grad_case4(B, C, dA, dB, dC);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (std::abs(goldenB[i][j] - dB[i][j]) >= eps1 && std::abs((goldenB[i][j] - dB[i][j]) / goldenB[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (std::abs(goldenC[i][j] - dC[i][j]) >= eps1 && std::abs((goldenC[i][j] - dC[i][j]) / goldenC[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


bool test_case5(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "A<16, 32>[i, j] =  B<16, 32, 4>[i, k, l] * C<32, 32>[k, j] * D<4, 32>[l, j];"
    // "dB<16, 32, 4>[i, k, l] = dA<16, 32>[i, j] * C<32, 32>[k, j] * D<4, 32>[l, j];"
    float dA[16][32] = {{0}};
    float dB[16][32][4] = {{{0}}};
    float C[32][32] = {{0}};
    float D[4][32] = {{0}};
    float golden[16][32][4] = {{{0}}};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            dA[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            C[i][j] = dis(gen);
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 32; ++j) {
            D[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            for (int k = 0; k < 32; ++k) {
                for (int l = 0; l < 4; ++l) {
                    golden[i][k][l] +=  dA[i][j] * C[k][j] * D[l][j];
                }
            }
        }
    }
    try {
        grad_case5(C, D, dA, dB);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 16; ++i) {
        for (int k = 0; k < 32; ++k) {
            for (int l = 0; l < 4; ++l) {
                if (std::abs(golden[i][k][l] - dB[i][k][l]) >= eps1 && std::abs((golden[i][k][l] - dB[i][k][l]) / golden[i][k][l]) >= eps2) {
                    std::cout << "Wrong answer\n";
                    return false;
                }
            }
        }
    }
    // correct
    return true;
}


bool test_case6(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "A<2, 8, 5, 5>[n, k, p, q] = B<2, 16, 7, 7>[n, c, p + r, q + s] * C<8, 16, 3, 3>[k, c, r, s];"
    // "dB<2, 16, 7, 7>[n, c, h, w] = select((h - p >= 0) && (w - q >= 0) && (h - p < 3) && (w - q < 3), dA<2, 8, 5, 5>[n, k, p, q] * C<8, 16, 3, 3>[k, c, h - p, w - q], 0.0);"
    float dA[2][8][5][5] = {{{{0}}}};
    float dB[2][16][7][7] = {{{{0}}}};
    float C[8][16][3][3] = {{{{0}}}};
    float golden[2][16][7][7] = {{{{0}}}};
    for (int n = 0; n < 2; ++n) {
        for (int c = 0; c < 8; ++c) {
            for (int h = 0; h < 5; ++h) {
                for (int w = 0; w < 5; ++w) {
                    dA[n][c][h][w] = dis(gen);
                }
            }
        }
    }
    for (int k = 0; k < 8; ++k) {
        for (int c = 0; c < 16; ++c) {
            for (int r = 0; r < 3; ++r) {
                for (int s = 0; s < 3; ++s) {
                    C[k][c][r][s] = dis(gen);
                }
            }
        }
    }
    // compute golden
    for (int n = 0; n < 2; ++n) {
        for (int k = 0; k < 8; ++k) {
            for (int p = 0; p < 5; ++p) {
                for (int q = 0; q < 5; ++q) {
                    for (int c = 0; c < 16; ++c) {
                        for (int h = 0; h < 7; ++h) {
                            for (int w = 0; w < 7; ++w) {
                                if ((h - p >= 0) && (w - q >= 0) && (h - p < 3) && (w - q < 3)) {
                                    golden[n][c][h][w] += dA[n][k][p][q] * C[k][c][h-p][w-q];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    try {
        grad_case6(C, dA, dB);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int n = 0; n < 2; ++n) {
        for (int k = 0; k < 8; ++k) {
            for (int h = 0; h < 7; ++h) {
                for (int w = 0; w < 7; ++w) {
                    if (std::abs(golden[n][k][h][w] - dB[n][k][h][w]) >= eps1 && std::abs((golden[n][k][h][w] - dB[n][k][h][w]) / golden[n][k][h][w]) >= eps2) {
                        std::cout << "Wrong answer\n";
                        return false;
                    }
                }
            }
        }
    }
    // correct
    return true;
}


bool test_case7(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "B<16, 32>[i, j] = A<32, 16>[j, i];"
    // "dA<32, 16>[j, i] = dB<16, 32>[i, j];"
    float dA[32][16] = {{0}};
    float dB[16][32] = {{0}};
    float golden[32][16] = {{0}};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            dB[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            golden[j][i] = dB[i][j];
        }
    }
    try {
        grad_case7(dB, dA);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 32; ++j) {
            if (std::abs(golden[j][i] - dA[j][i]) >= eps1 && std::abs((golden[j][i] - dA[j][i]) / golden[j][i]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


bool test_case8(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "B<32>[i] = A<2, 16>[i//16, i%16];"
    // "dA<2, 16>[i, j] = dB<32>[i * 16 + j];"
    float dA[2][16] = {{0}};
    float dB[32] = {0};
    float golden[2][16] = {{0}};
    for (int i = 0; i < 32; ++i) {
        dB[i] = dis(gen);
    }
    // compute golden
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 16; ++j) {
            golden[i][j] = dB[i * 16 + j];
        }
    }
    try {
        grad_case8(dB, dA);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 16; ++j) {
            if (std::abs(golden[i][j] - dA[i][j]) >= eps1 && std::abs((golden[i][j] - dA[i][j]) / golden[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


bool test_case9(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "B<4, 6>[i, j] = A<4>[i];"
    // "dA<4>[i] = dB<4, 6>[i, j];"
    float dA[4] = {0};
    float dB[4][6] = {{0}};
    float golden[4] = {0};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 6; ++j) {
            dB[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 6; ++j) {
            golden[i] += dB[i][j];
        }
    }
    try {
        grad_case9(dB, dA);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 4; ++i) {
        if (std::abs(golden[i] - dA[i]) >= eps1 && std::abs((golden[i] - dA[i]) / golden[i]) >= eps2) {
            std::cout << "Wrong answer\n";
            return false;
        }
    }
    // correct
    return true;
}


bool test_case10(std::mt19937 &gen, std::uniform_real_distribution<float> &dis) {
    // "A<8, 8>[i, j] = (B<10, 8>[i, j] + B<10, 8>[i + 1, j] + B<10, 8>[i + 2, j]) / 3.0;"
    // "dB<10, 8>[i, j] = (select(i >= 0 && i < 8, dA<8, 8>[i, j], 0.0) + select(i >= 1 && i < 9, dA<8, 8>[i - 1, j], 0.0) + select(i >= 2 && i < 10, dA<8, 8>[i - 2, j], 0.0)) / 3.0"
    float dA[8][8] = {{0}};
    float dB[10][8] = {{0}};
    float golden[10][8] = {{0}};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            dA[i][j] = dis(gen);
        }
    }
    // compute golden
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 8; ++j) {
            golden[i][j] = ((i >= 0 && i < 8 ? dA[i][j] : 0.0) + (i >= 1 && i < 9 ? dA[i-1][j] : 0.0) + (i >= 2 && i < 10 ? dA[i-2][j] : 0.0)) / 3.0;
        }
    }
    try {
        grad_case10(dA, dB);
    } catch (...) {
        std::cout << "Failed because of runtime error\n";
        return false;
    }

    // check
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (std::abs(golden[i][j] - dB[i][j]) >= eps1 && std::abs((golden[i][j] - dB[i][j]) / golden[i][j]) >= eps2) {
                std::cout << "Wrong answer\n";
                return false;
            }
        }
    }
    // correct
    return true;
}


int main() {
    std::random_device rd;  // get random seed
    std::mt19937 gen(rd()); // standard
    std::uniform_real_distribution<float> dis(-10, 10);
    std::cout << "Random distribution ready\n";
    int num_pass = 0;
    float score[] = {0, 2.25, 4.5, 6.75, 9, 10.5, 12, 12.75, 13.5, 14.25, 15};
    // cases
    std::cout << "Case 1 ";
    bool res = test_case1(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 2 ";
    res = test_case2(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 3 ";
    res = test_case3(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 4 ";
    res = test_case4(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 5 ";
    res = test_case5(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 6 ";
    res = test_case6(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 7 ";
    res = test_case7(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 8 ";
    res = test_case8(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 9 ";
    res = test_case9(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Case 10 ";
    res = test_case10(gen, dis);
    if (res) {
        std::cout << "Success!\n";
        num_pass += 1;
    }
    std::cout << "Totally pass " << num_pass << " out of 10 cases.\n";
    std::cout << "Score is " << score[num_pass] << ".\n";
    return 0;
}
