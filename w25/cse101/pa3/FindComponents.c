/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA3
* FindComponents.c
* Takes an Input File and an output file then
* runs DFS on G and G^T to determine G's strong components
* Adjancency Lists and Strong Components are Written out
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
    while (fscanf(inFile, "%d %d", &u, &v) == 2) {
        if (u == 0 && v == 0) break;
        addArc(G, u, v);
    }
    fprintf(outFile, "Adjacency list representation of G:\n");
    printGraph(outFile, G);


    // First DFS on G
    List S = newList();
    for (int i = 1; i <= Order; i++) {
        append(S, i);
    }
    DFS(G, S);  // First DFS to get finish times

    // Create transpose graph
    Graph T = transpose(G);
    // Second DFS on transpose, using reverse finish times
    DFS(T, S); 

    // Print strong components
    int component_count = 0;
    List strong_components = newList();
    
    moveFront(S);
    while(index(S) != -1){
        if (getParent(T, get(S)) == NIL) {  // Start of a new component
            component_count++;
            prepend(strong_components, get(S));
        }
        moveNext(S);
    }

    fprintf(outFile, "\nG contains %d strongly connected components:\n", component_count);

    int component_num = 1;
    moveBack(S);
    for(moveFront(strong_components); index(strong_components) >= 0; moveNext(strong_components)){
        fprintf(outFile, "Component %d: ", component_num++);
        List components = newList();
        while(index(S) >= 0){
            if(getParent(T,get(S)) == NIL){
                prepend(components, get(S));
                movePrev(S);
                break;
            }
            prepend(components, get(S));
            movePrev(S);
        }
        printList(outFile, components);
        fprintf(outFile, "\n");
        freeList(&components);
    }



    freeList(&strong_components);
    // Free memory
    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    
    // Close files
    fclose(inFile);
    fclose(outFile);

    return 0;
}