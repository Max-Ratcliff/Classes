# CSE 101 Winter 25 Programming assignment \#4:
## Files:
### List.h
Header file for the List ADT, acts as the interface for manipulating the Linked List.
### List.c 
Implementation of the List ADT.
### Matrix.h
Header file for the Matrix ADT, acts as the interface for manipulating a sparse matrix representation with operations optimized for sparcity
### Matrix.c
Implementation of the Matrix ADT. 
### ListTest.c
Tests the implementation of the List ADT by using all of the manipulation functions and asserting that the result is as expected.
### MatrixTest.c
Tests the implementation of the Matrix ADT by using all of the manipulation functions and asserting that the result is as expected.
### Sparse.c
Client Module, takes two file paths (inFile and outFile) as input and uses the Matrix ADT to print out a variety of Matrix operations: $𝐴,\ 𝐵,\ (1.5)𝐴,\ 𝐴 + 𝐵,\ 𝐴 + 𝐴,\ 𝐵 − 𝐴,\ 𝐴 − 𝐴,\ 𝐴𝑇,\ 𝐴𝐵\ and\ 𝐵^2$
### Makefile
Used to compile source files into binaries
- `make` will create the executable `./Sparse `
- `make all` will make: `./Sparse`, `./MatrixTest`, `./ListTest`, `./MatrixClient`, and `./ListClient`
- `make client` will make: `./MatrixClient` and `./ListClient`
- `make test` will make: `./MatrixTest` and `./ListTest`
- `make clean` deletes all executables and binaries
### README.md 
this file :\), explains the purpose of each file and how to compile and run the program