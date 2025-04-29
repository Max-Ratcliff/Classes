# CSE 101 Winter 25 Programming assignment \#5:
## Files:
### List.h
Header file for the List ADT, acts as the interface for manipulating the Linked List.
### List.cpp 
Implementation of the List ADT.
### ListTest.cpp
Tests the implementation of the List ADT by using all of the manipulation functions and asserting that the result is as expected.
### Shuffle.cpp
Client Module, takes an integer as an input, and chekcs how many shuffles it takes decks up to that size to return to their original state
### Makefile
Used to compile source files into binaries
- `make` will create the executable `./Shuffle `
- `make all` will make: `./Shuffle`, `./ListTest`, `./ListClient`
- `make client` will make: `./ListClient`
- `make test` will make: `./ListTest`
- `make clean` deletes all executables and binaries
### README.md 
this file :\), explains the purpose of each file and how to compile and run the program