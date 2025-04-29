/****************************************************
 * Max Ratcliff
 * mwratcli
 * 2025 Winter CSE101 PA4
 * MatrixTest.c 
 * 
 * A testing client for the Matrix ADT.
 * This program tests every function provided in Matrix.c,
 ****************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <assert.h>
 #include "Matrix.h"  
 
 int main(void) {
 
     /***** Test newMatrix, size, NNZ, and printMatrix *****/
     int n = 5;
     Matrix A = newMatrix(n);
     assert(size(A) == n);
     assert(NNZ(A) == 0);
     printMatrix(stdout, A);
     printf("\n");
 
     //testing adding new entries
     changeEntry(A, 1, 1, 1.0);  // A[1,1] = 1.0
     changeEntry(A, 1, 3, 2.0);  // A[1,3] = 2.0
     changeEntry(A, 2, 2, 3.0);  // A[2,2] = 3.0
     changeEntry(A, 3, 1, 4.0);  // A[3,1] = 4.0
     changeEntry(A, 4, 5, 5.0);  // A[4,5] = 5.0
     changeEntry(A, 5, 4, 6.0);  // A[5,4] = 6.0
     assert(NNZ(A) == 6);
     printMatrix(stdout, A);
     printf("\n");
 
     //testing changing an exisitng entry
     changeEntry(A, 1, 3, 7.0);
     assert(NNZ(A) == 6);  // NNZ should remain the same.
     printMatrix(stdout, A);
     printf("\n");
 
     // Delete an entry by setting it to 0: remove A[3,1].
     changeEntry(A, 3, 1, 0.0);
     assert(NNZ(A) == 5);
     printMatrix(stdout, A);
     printf("\n");
 
     //test copy
     // Copy matrix A into B.
     Matrix B = copy(A);
     // They should be equal.
     assert(equals(A, B));
 
     // Modify B and verify that A and B are no longer equal.
     changeEntry(B, 2, 2, 10.0);  // Change B[2,2] from 3.0 to 10.0.
     assert(!equals(A, B));
 
     //test makeZero 
     makeZero(A);
     assert(NNZ(A) == 0);
 
     // test matrix operations
     Matrix C = newMatrix(n);
     changeEntry(C, 1, 2, 2.0);  // C[1,2] = 2.0
     changeEntry(C, 2, 3, 3.0);  // C[2,3] = 3.0
     changeEntry(C, 3, 4, 4.0);  // C[3,4] = 4.0
     changeEntry(C, 4, 5, 5.0);  // C[4,5] = 5.0
     changeEntry(C, 5, 1, 6.0);  // C[5,1] = 6.0
     printMatrix(stdout, C);
     printf("\n");
 
     // test scalar mult
     Matrix D = scalarMult(2.0, C);
     printMatrix(stdout, D);
     printf("\n");
 
     //test sum
     Matrix E = sum(C, D);
     printMatrix(stdout, E);
     printf("\n");
 
    //test subtraction
     Matrix F = diff(D, C);
     printMatrix(stdout, F);
     printf("\n");
 
     //test transpose
     Matrix C_T = transpose(C);
     printMatrix(stdout, C_T);
     printf("\n");
 
     //matrix transposed twice shouldnt change it
     Matrix C_TT = transpose(C_T);
     assert(equals(C, C_TT));
 
     // test product
     Matrix G = product(C, C_T);
     printMatrix(stdout, G);
     printf("\n");
 
 
     // clean up memory
     freeMatrix(&A);
     freeMatrix(&B);
     freeMatrix(&C);
     freeMatrix(&D);
     freeMatrix(&E);
     freeMatrix(&F);
     freeMatrix(&C_T);
     freeMatrix(&C_TT);
     freeMatrix(&G);
 
     printf("All Matrix tests passed successfully!\n");
     return 0;
 }