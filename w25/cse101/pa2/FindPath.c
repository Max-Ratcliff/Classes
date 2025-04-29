/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA2
* FindPath.c 
* Client Program that reads in the order of a graph and it's adjacency matrix
* And then reads in a series of source and target nodes to perform BFS on and determine the shortest path
***/ 
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

int main(int argc, char* argv[]) {
    FILE *inFile = NULL, *outFile = NULL;
    int Order, u, v;

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
    // Read number of vertices
    if (fscanf(inFile, "%d", &Order) != 1) {
        fprintf(stderr, "invalid number of vertices\n");
        exit(1);
    }

    Graph G = newGraph(Order);

    // Read in edges to create graph and print adjacency list
    // while reading edges in the format
    //                     u   v
    while (fscanf(inFile, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) break; // break when 0 0 is encountered signaling next part of program
        addEdge(G, u, v);
    }

    // Print adjacency list
    printGraph(outFile, G);

    // second part
    List L = newList();
    // read in source vertices and destination vertices
    //                     u   v
    while (fscanf(inFile, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) break;
        BFS(G, u);
        int dist = getDist(G, v);

        if (dist == INF) {
            fprintf(outFile, "\nThe distance from %d to %d is infinity\n", u, v);
            fprintf(outFile, "No %d-%d path exists\n", u, v);
        } else {
            fprintf(outFile, "\nThe distance from %d to %d is %d\n", u, v, dist);
            fprintf(outFile, "A shortest %d-%d path is: ", u, v);
            getPath(L, G, v); // save the path to a list
            printList(outFile, L); // print the path
            fprintf(outFile, "\n");
            clear(L); // clear the list for next path
        }
    }

    // Clean up
    // free memory
    freeList(&L);
    freeGraph(&G);
    //close files
    fclose(inFile);
    fclose(outFile);
    return 0;
}