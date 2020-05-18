#ifndef PROJECT1_RUN_H
#define PROJECT1_RUN_H

void kernel_example(float (&B)[32][16], float (&C)[32][16], float (&A)[32][16]);
void kernel_case1(float (&A)[32][16]);
void kernel_case2(float (&A)[16][8]);
void kernel_case3(int (&B)[16][32], int (&C)[16][32], int (&A)[16][32]);
void kernel_case4(float (&B)[16][32], float (&C)[32][32], float (&A)[16][32]);
void kernel_case5(float (&B)[16][32], float (&C)[32][32], float (&D)[16][32],
    float &alpha, float &beta, float (&A)[16][32]);
void kernel_case6(float (&B)[2][16][7][7], float (&C)[8][16][3][3], float (&A)[2][8][5][5]);
void kernel_case7(float (&A)[32][16], float (&B)[16][32]);
void kernel_case8(float (&B)[8][16], float (&A)[8][2][16]);
void kernel_case9(float (&B)[16][32][8], float (&C)[32][32], float (&D)[8][32], float (&A)[16][32]);
void kernel_case10(float (&B)[10][10], float (&A)[8][8]);

#endif  // PROJECT1_RUN_H