/****************************************************
 * Max Ratcliff
 * mwratcli
 * 2025 Winter CSE101 PA4
 * Sparse.c 
 * 
 * client for the Matrix ADT.
 ****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Matrix.h"

int main(int argc, char* argv[]) {
    FILE *inFile = NULL, *outFile = NULL;

    //make sure both files are provided
    if (argc != 3) {
        fprintf(stderr, "Requires 2 arguments\n");
        exit(1);
    }

    //open infile for reading and outfile for writing
    inFile = fopen(argv[1], "r");
    if (inFile == NULL) {
        fprintf(stderr, "Could not open input file: %s\n", argv[1]);
        exit(1);
    }
    outFile = fopen(argv[2], "w");
    if (outFile == NULL) {
        fprintf(stderr, "Could not open output file: %s\n", argv[2]);
        exit(1);
    }
    int n, a, b; //values to read from the first line of the fike
    // where n is the size of the nxn matrices A and B
    // and a and b counts the number of lines that the program should
    // read in to store in their respective matrices

    // Read first line of file
    // save matrix sizes and NNZ counts
    if (fscanf(inFile, "%d %d %d", &n, &a, &b) != 3) {
        fprintf(stderr, "expected 3 integers\n");
        exit(1);
    }

    // create matrixes
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    int i, j; // row, column
    double val; // value

    for(int iter = 0; iter < a; iter++){ //loop through the number of entries to add to a 
        if(fscanf(inFile, "%d %d %lf", &i, &j, &val) != 3){
            fprintf(stderr, "expected 2 integers and a double while filling A\n");
            exit(1);
        }
        changeEntry(A, i, j, val);
    }

    fscanf(inFile, "\n"); // eat the new line

    for(int iter = 0; iter < b; iter++){ //loop through the number of entries to add to a 
        if(fscanf(inFile, "%d %d %lf", &i, &j, &val) != 3){
            fprintf(stderr, "expected 2 integers and a double while filling B\n");
            exit(1);
        }
        changeEntry(B, i, j, val);
    }

    assert(NNZ(A) == a);
    assert(NNZ(B) == b);

    fprintf(outFile, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(outFile, A);
    fprintf(outFile, "\n");

    fprintf(outFile, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(outFile, B);
    fprintf(outFile, "\n");

    Matrix timesA = scalarMult(1.5, A);
    fprintf(outFile, "(1.5)*A =\n");
    printMatrix(outFile, timesA);
    fprintf(outFile, "\n");

    Matrix ApB = sum(A,B);
    fprintf(outFile, "A+B =\n");
    printMatrix(outFile, ApB);
    fprintf(outFile, "\n");

    Matrix ApA = sum(A, A);
    fprintf(outFile, "A+A =\n");
    printMatrix(outFile, ApA);
    fprintf(outFile, "\n");

    Matrix BmA = diff(B,A);
    fprintf(outFile, "B-A =\n");
    printMatrix(outFile, BmA);
    fprintf(outFile, "\n");

    Matrix AmA = diff(A,A);
    fprintf(outFile, "A-A =\n");
    printMatrix(outFile, AmA);
    fprintf(outFile, "\n");

    Matrix A_T = transpose(A);
    fprintf(outFile, "Transpose(A) =\n");
    printMatrix(outFile, A_T);
    fprintf(outFile, "\n");

    Matrix AtimesB = product(A, B);
    fprintf(outFile, "A*B =\n");
    printMatrix(outFile, AtimesB);
    fprintf(outFile, "\n");

    Matrix BtimesB = product(B, B);
    fprintf(outFile, "B*B =\n");
    printMatrix(outFile, BtimesB);
    fprintf(outFile, "\n");


    // Free memory
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&timesA);
    freeMatrix(&ApB);
    freeMatrix(&ApA);
    freeMatrix(&BmA);
    freeMatrix(&AmA);
    freeMatrix(&A_T);
    freeMatrix(&AtimesB);
    freeMatrix(&BtimesB);

    // Close files
    fclose(inFile);
    fclose(outFile);

    return 0;
}