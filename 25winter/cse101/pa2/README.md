# CSE 101 Winter 25 Programming assignment \#2:
## Files:
### List.h
Header file for the List ADT, acts as the interface for manipulating the Linked List.
### List.c 
Implementation of the List ADT.
### Graph.h
Header file for the Graph ADT, acts as the interface for manipulating an Adjacency matrix with a built in BFS algorithm.
### Graph.c
Implementation of the Graph ADT.
### GraphTest.c
Tests the implementation of the Graph ADT by using all of the manipulation functions and asserting that the result is as expected.
### FindPath.c
Client Module, takes two file paths (inFile and outFile) as input and uses the Graph ADT to print out an adjacency matrix and a shortest paths between two nodes.
### Makefile
Used to compile source files into binaries, `make` will create the executable `./FindPath `, and `make all` will make both `./FindPath` and `./GraphTest`. `/make clean` will clear all binaries.
### README.md 
this file :), explains the purpose of each file and how to compile and run the program