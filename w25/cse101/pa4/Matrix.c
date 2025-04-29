/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA4
* Matrix.c 
* Implementation of a Matrix ADT optimized for sparsity.
* the ADT will function as intended on both dense and sparse matrices
***/ 

#include "Matrix.h" 
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct EntryObj{
    int column;
    double value;
} EntryObj;

typedef EntryObj* Entry;

typedef struct MatObj{
    int dims; // the dimensions of the n x n square matrix
    int NNZ; // the number of non zero entries
    List* rows; // an array of lists representing the matrix where each index contains a row
} MatObj;

// Creates and returns a new Entry with (collumn, value) pair
// Returns reference to new Entry object
Entry newEntry(int col, double val) {
    Entry nEntry = malloc(sizeof(EntryObj));
    nEntry->column = col;
    nEntry->value = val;
    return nEntry;
}

// Frees heap memory pointed to by *pE, sets *pE to NULL
void freeEntry(Entry* pE) {
    if(pE != NULL && *pE != NULL) {
        free(*pE);
        *pE = NULL;
    }
}

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
    Matrix M = malloc(sizeof(MatObj));
    M->dims = n;
    M->NNZ = 0;
    M->rows = calloc(n+1, sizeof(List));

    for (int i = 1; i <= n; i++){ // rows are counted from 1 to n+1
        M->rows[i] = newList();
    }
    return M;
}
// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    if(pM != NULL && *pM != NULL){
        int n = size(*pM);
        for (int i = 1; i <= n; i++){
            List i_row = (*pM)->rows[i];
            moveFront(i_row);
            while(index(i_row) != -1){ // iterate over list to free entrys 
                Entry E = get(i_row);
                freeEntry(&E);// free each entry
                moveNext(i_row);
            }
            freeList(&i_row); // free each Entry List
        }
        free((*pM)->rows); // free the rows array
        free(*pM); // free the matrix
        *pM = NULL; // nullify pointer
    }
}
// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M){
    assert(M != NULL);
    return M->dims;
}
// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    assert(M != NULL);
    return M->NNZ;
}
// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
    assert(A != NULL);
    assert(B != NULL);
    // check the dimensions and number of non zero values match
    if (size(A) != size(B) || NNZ(A) != NNZ(B)){ 
        return 0;
    }
    // check that the values match
    int n = size(A);
    for(int i = 1; i <= n; i++){
        List A_row = A->rows[i];
        List B_row = B->rows[i];
        moveFront(A_row);
        moveFront(B_row);
        // check that rows are equal by looping each one
        while(index(A_row) != -1 && index(B_row) != -1){
            // check if entries are the same in each row
            Entry A_entry = get(A_row);
            Entry B_entry = get(B_row);
            if(A_entry->column != B_entry->column
            || A_entry->value != B_entry->value){
                return 0;
            }
            moveNext(A_row);
            moveNext(B_row);
        }
    }
    

    return 1;
}
// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
    assert(M != NULL);
    //set nnz to the null state
    M->NNZ = 0;

    //re-set row list
    int n = size(M);
    for (int i = 1; i <= n; i++){ 
        List i_row = M->rows[i];
        moveFront(i_row);
        while(index(i_row) != -1){ // iterate over list to free entrys 
            Entry E = get(i_row);
            freeEntry(&E);// free each entry
            moveNext(i_row);
        }
       clear(M->rows[i]); // reset list to empty
    }
}
// helper function to insert an entry into the columns list sorted
void insertSorted(List L, Entry x) {
    if (length(L) == 0) {
        append(L, x);
        return;
    }
    
    moveFront(L); // move cursor to front
    while (index(L) != -1) { // iterate through list
        Entry iE = get(L); //ith entry
        if (x->column < iE->column) { // x is comes before ith entry
            insertBefore(L, x); // insert x before cursor
            return;
        }
        moveNext(L); // move cursor to next node
    }
    append(L, x); // x is the biggest element
    return;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    assert(M != NULL);
    assert(1<=i && i <=size(M));
    assert(1<=j && j <=size(M));
    // enter the ith row to search for a matching jth column
    List i_row = M->rows[i];
    moveFront(i_row);
    while (index(i_row) != -1){
        Entry E = get(i_row);
        if(E->column == j){
            if (x == 0.0){ // if we set to zero 
                freeEntry(&E); // free the item under the cursor
                delete(i_row); // delete the column under the cursor
                M->NNZ--; // reduce the number of non-zero values
            }else{
                E->value = x; // update val
            }
            return; // element has been changed, break out of function
        }
        moveNext(i_row);
    } 
    // if you dont return before reaching the end insert the (col, val) pair
    if(x != 0.0){
        insertSorted(i_row, newEntry(j, x));
        M->NNZ++;
    }
    
}
// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
    assert(A != NULL);
    int n = size(A);
    Matrix B = newMatrix(n);
    B->NNZ=A->NNZ;
    for(int i = 1; i <= n; i++){
        List A_row = A->rows[i];
        List B_row = B->rows[i];
        moveFront(A_row);
        while(index(A_row) != -1){
            Entry E = get(A_row);
            append(B_row, newEntry(E->column, E->value));
            moveNext(A_row);
        }
    }
    return B;
}
// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
    // defined as every (i, j) Entry of A = the (j, i) of A^T
    int n = size(A); // A != NULL is checked in size()
    Matrix A_T = newMatrix(n);
    A_T->NNZ=A->NNZ;  // Non Zero count remains constant

    for(int i = 1; i <= n; i++){
        List i_row = A->rows[i];
        moveFront(i_row);

        while (index(i_row) != -1){
            Entry E = get(i_row); // Entry (i,j) of matrix A
            // A_T is a new matrix and change entry will always append so we
            // changeEntry(A_T, E->column, i, E->value); // set to entry (j, i) of A_T
            int j = E->column;
            append(A_T->rows[j], newEntry(i, E->value));
            moveNext(i_row);
        }
    }
    return A_T;
}
// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
    int n = size(A); // Nullity is checked
    Matrix xA = newMatrix(n);
    for(int i = 1; i <= n; i++){
        List i_row = A->rows[i];
        moveFront(i_row);
        while (index(i_row) != -1){
            Entry E = get(i_row); // Entry (i,j) of matrix A
            double nVal = x * E->value;
            changeEntry(xA, i , E->column, nVal);
            moveNext(i_row);
        }
    }
    return xA; 
}
// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
    int n = size(A); // Nullity of A is checked
    assert(n == size(B)); // Nullity of B is checked as well as asserting dimensions match
    
    if(A == B){ // both matrices point to the same object (A+A)
        return scalarMult(2.0, A); // return 2*A, this saves on runtime too becasue scalarMult is O(n+a) and sum is O(n+a+b)
    }

    Matrix sum = newMatrix(n); // initialized sum to be a copy of A
    for (int i = 1; i <= n; i++) {
        List A_row = A->rows[i];
        List B_row = B->rows[i];

        moveFront(A_row);
        moveFront(B_row);

        while (index(B_row) != -1 || index(A_row) != -1) {
            if(index(A_row) == -1){
                // all other more entry's in A are 0
                Entry B_entry = get(B_row);
                int B_col = B_entry->column;
                double B_val = B_entry->value;
                changeEntry(sum, i, B_col,(0 + B_val));
                moveNext(B_row);
                continue;
            }
            if(index(B_row) == -1){
                Entry A_entry = get(A_row);
                int A_col = A_entry->column;
                double A_val = A_entry->value;
                // all other more entry's in B are 0
                changeEntry(sum, i, A_col,(A_val + 0));
                moveNext(A_row);
                continue;
            }

            Entry B_entry = get(B_row);
            int B_col = B_entry->column;
            double B_val = B_entry->value;

            Entry A_entry = get(A_row);
            int A_col = A_entry->column;
            double A_val = A_entry->value;
            

            if (A_col == B_col) {
                // both values are non 0
                changeEntry(sum, i, B_col, (A_val + B_val));
                moveNext(A_row);
                moveNext(B_row);
            } else if (A_col < B_col) {
                // at (i,j) B = 0 and so set sum = A 
                changeEntry(sum, i, A_col, (A_val + 0));
                moveNext(A_row); // interate A to catch up to B
            } else if (A_col > B_col){ 
                // at (i, j) A = sum = 0 so set sum = B
                changeEntry(sum, i, B_col, (0 + B_val));
                moveNext(B_row);
            }else{
                // this shouldnt be reached
                fprintf(stderr, "wow this isnt supposed to happen");
                freeMatrix(&sum); // clean memory
                exit(1); // exit program
            }
        }
    }
    return sum; 
}
// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    Matrix B_neg = scalarMult(-1.0,B);
    Matrix summed = sum(A, B_neg);
    freeMatrix(&B_neg);
    return summed;
}

// computes the vector dot product of two matrix rows represented by Lists P and 
double vectorDot(List P, List Q){
    double dot = 0.0;
    moveFront(P);
    moveFront(Q);
    while(index(P) != -1 && index(Q) != -1){
        Entry E_P = get(P);
        Entry E_Q = get(Q);
        if(E_P->column == E_Q->column){
            dot += E_P->value*E_Q->value; // both Entries are non 0 
            moveNext(P);
            moveNext(Q);
        } else if (E_P->column < E_Q->column){ // at i, Q=0
            // x*0 = 0
            moveNext(P); //advance just P to try to catch up to Q
        } else if(E_P->column > E_Q->column){ // at i P = 0
            // 0*x = 0
            moveNext(Q); // advance just Q to try to catch up to P
        }else{
            // this shouldnt be reached
            fprintf(stderr, "wow this isnt supposed to happen");
            exit(1);
        }
    }
    return dot;
}
// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    int n = size(A); // Nullity of A is checked
    assert(n == size(B)); // Nullity of B is checked as well as asserting dimensions match
    Matrix prod = newMatrix(n); // initialize prod 
    // for every (i,j) entry in prod compute the dot product of row i of A and col j of B
    Matrix B_T = transpose(B);
    for(int i = 1; i <= n; i++){
        if(length(A->rows[i]) !=0){
            for(int j = 1; j <= n; j++){
                double dot = vectorDot(A->rows[i], B_T->rows[j]);
                if (dot != 0.0){
                    changeEntry(prod, i, j, dot);
                }
            }
        }
    }
    freeMatrix(&B_T);
    return prod;
}
// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
    assert(M != NULL);
    assert(out != NULL);

    int n = size(M);
    for(int i = 1; i <= n; i++){
        List i_row = M->rows[i];
        if(length(i_row) != 0){ // zero row
            fprintf(out, "%d: ", i); // print row number
            //print list function doesnt work becasue it doesnt know the type of listElement
            moveFront(i_row);
            while(index(i_row) != -1){
                Entry E = get(i_row);
                fprintf(out, "(%d, %.1f) ", E->column, E->value);
                moveNext(i_row);
            }
            fprintf(out, "\n");
        }   
    }
}