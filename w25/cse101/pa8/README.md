# CSE 101 Winter 25 Programming assignment \#7:
## Files:
### Dictionary.h
Header file for the Dictionary ADT, acts as the interface for manipulating the Red-Black Binary Search Tree
### Dictionary.cpp 
Implementation of the Dictionary ADT.
### DictionaryTest.cpp
Tests the implementation of the Dictionary ADT by using all of the manipulation functions and asserting that the result is as expected.

### Order.cpp
Client Module, That reads in lines of a text file as an input and perform a pre-order tree walk on the dictionary constructed
### WordFrequency.cpp
Client Module, That reads in lines of a text file as an input and counts the occurance of each word that apear in the file
### Makefile
Used to compile source files into binaries
- `make` will create the executable `./WordFrequency`
- `make all` will make: `./WordFrequency `, `./Order`, `./DictionaryTest`, `./DicionaryClient`
- `make client` will make: `./DictionaryClient`
- `make test` will make: `./DictionaryTest`
- `make clean` deletes all executables and binaries
### README.md 
this file :\), explains the purpose of each file and how to compile and run the program