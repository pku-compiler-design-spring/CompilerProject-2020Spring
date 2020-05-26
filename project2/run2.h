#ifndef PROJECT2_RUN_H
#define PROJECT2_RUN_H

void grad_case1(float (&B)[4][16], float (&dC)[4][16], float (&dA)[4][16]);
void grad_case2(float (&A)[4][16], float (&dB)[4][16], float (&dA)[4][16]);
void grad_case3(float (&B)[16][16], float (&dC)[4][16], float (&dA)[4][16]);
void grad_case4(float (&B)[16][32], float (&C)[32][32], float (&dA)[16][32], float (&dB)[16][32], float (&dC)[32][32]);
void grad_case5(float (&C)[32][32], float (&D)[4][32], float (&dA)[16][32], float (&dB)[16][32][4]);
void grad_case6(float (&C)[8][16][3][3], float (&dA)[2][8][5][5], float (&dB)[2][16][7][7]);
void grad_case7(float (&dB)[16][32], float (&dA)[32][16]);
void grad_case8(float (&dB)[32], float (&dA)[2][16]);
void grad_case9(float (&dB)[4][6], float (&dA)[4]);
void grad_case10(float (&dA)[8][8], float (&dB)[10][8]);

#endif  // PROJECT2_RUN_H