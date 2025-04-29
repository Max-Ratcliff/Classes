# CSE 101 Winter 25 Programming assignment \#3:
## Files:
### List.h
Header file for the List ADT, acts as the interface for manipulating the Linked List.
### List.c 
Implementation of the List ADT.
### Graph.h
Header file for the Graph ADT, acts as the interface for manipulating an Adjacency matrix with a built in DFS algorithm.
### Graph.c
Implementation of the Graph ADT. with DFS and BFS methods, although everything BFS specific is set to be private by excluding it from Graph.h, to track time across visit() calls for DFS, I chose passing an adress to visit() and keeping its scope to DFS()
### GraphTest.c
Tests the implementation of the Graph ADT by using all of the manipulation functions and asserting that the result is as expected.
### FindComponents.c
Client Module, takes two file paths (inFile and outFile) as input and uses the Graph ADT to print out an adjacency matrix and all of the strongly connected components given by: $X \subseteq V \text{ is strongly connected IFF } \forall e \in X, \forall v \in (X-\{e\}) \text{ there is a path from } v \text{ to } e$
### Makefile
Used to compile source files into binaries, `make` will create the executable `./FindComponents `, and `make all` will make both `./FindComponents` and `./GraphTest`. `/make clean` will clear all binaries.
### README.md 
this file :\), explains the purpose of each file and how to compile and run the program