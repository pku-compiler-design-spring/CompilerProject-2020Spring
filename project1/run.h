#ifndef PROJECT1_RUN_H
#define PROJECT1_RUN_H

void kernel_example(float (&B)[32][16], float (&C)[32][16], float (&A)[32][16]);
void kernel_case1(float (&A)[32][16]);
void kernel_case4(float (&B)[16][32], float (&C)[32][32], float (&A)[16][32]);
void kernel_case5(float (&B)[16][32], float (&C)[32][32], float (&D)[16][32],
    float &alpha, float &beta, float (&A)[16][32]);
void kernel_case6(float (&B)[2][16][7][7], float (&C)[8][16][3][3], float (&A)[2][8][5][5]);
void kernel_case7(float (&A)[32][16], float (&B)[16][32]);
void kernel_case10(float (&B)[10][10], float (&A)[8][8]);

#endif  // PROJECT1_RUN_H