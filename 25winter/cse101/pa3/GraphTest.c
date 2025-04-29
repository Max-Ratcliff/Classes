/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA3
* GraphTest.c 
* Tests all of the methods in Graph.c (DFS-specific)
***/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "Graph.h"
#include "List.h"

int main() {
    List L = newList();
    Graph G = NULL;
    Graph GT = NULL; // Transpose graph

    // Test newGraph() and accessors
    G = newGraph(8);
    assert(getOrder(G) == 8);
    assert(getSize(G) == 0);
    printGraph(stdout, G);
    printf("\n");

    // Test adding arcs (directed graph for DFS)
    addArc(G, 1, 2);
    addArc(G, 1, 3);
    addArc(G, 2, 4);
    addArc(G, 2, 5);
    addArc(G, 3, 4);
    addArc(G, 4, 5);
    addArc(G, 5, 6);
    addArc(G, 6, 7);
    addArc(G, 7, 8);
    assert(getSize(G) == 9);  // Test size after adding arcs
    printGraph(stdout, G);
    printf("\n");

    // Prepare list for DFS (vertices 1 to 8)
    for (int i = 1; i <= getOrder(G); i++) {
        append(L, i);
    }

    // Run DFS
    DFS(G, L);
    printf("After DFS on G:\n");
    printGraph(stdout, G);
    printf("\n");

    // Check discovery and finish times
    for (int i = 1; i <= getOrder(G); i++) {
        printf("Vertex %d: Discover = %d, Finish = %d, Parent = %d\n",
               i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
        assert(getDiscover(G, i) != UNDEF);
        assert(getFinish(G, i) != UNDEF);
    }

    // Transpose the graph
    GT = transpose(G);
    printf("\nTranspose of G:\n");
    printGraph(stdout, GT);
    printf("\n");

    // Run DFS on the transpose graph in the order of decreasing finish times
    DFS(GT, L);
    printf("After DFS on GT (Transpose):\n");
    printGraph(stdout, GT);
    printf("\n");

    // Check strongly connected components using parent pointers
    printf("Strongly Connected Components:\n");
    int componentCount = 0;
    moveFront(L);
    while (index(L) != -1) {
        int v = get(L);
        if (getParent(GT, v) == NIL) {
            componentCount++;
            printf("Component %d: ", componentCount);
        }
        printf("%d ", v);
        moveNext(L);
        if (index(L) == -1 || getParent(GT, get(L)) == NIL) {
            printf("\n");
        }
    }

    // Clean up all allocated memory
    freeList(&L);
    freeGraph(&G);
    freeGraph(&GT);

    printf("All DFS-related tests passed!\n");
    return 0;
}