/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA2
* GraphTest.c 
* Tests all of the methods in Graph.c
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include "Graph.h"
#include "List.h"

int main() {
    List L = newList();
    Graph G = NULL;
    Graph D = NULL;

    // Test newGraph() and accessors
    G = newGraph(8);
    assert(getOrder(G) == 8);
    assert(getSize(G) == 0);
    assert(getSource(G) == NIL); 
    assert(getParent(G, 1) == NIL); 
    assert(getDist(G, 1) == INF); 
    printGraph(stdout, G);
    printf("\n");

    //test adding edges
    addEdge(G, 1, 2);
    addEdge(G, 1, 3);
    addEdge(G, 2, 4);
    addEdge(G, 2, 5);
    addEdge(G, 3, 4);
    addEdge(G, 4, 5);
    addEdge(G, 5, 6);
    addEdge(G, 6, 7);
    addEdge(G, 7, 8);
    assert(getSize(G) == 9);  // test size after adding edges
    printGraph(stdout, G);
    printf("\n");

    BFS(G, 1);
    assert(getSource(G) == 1);
    assert(getDist(G, 5) == 2);   // testing distance
    assert(getDist(G, 8) == 5); 
    assert(getParent(G, 4) == 2); // testing parent
    assert(getParent(G, 3) == 1);

    // test getpath
    getPath(L, G, 8);  // testing path to farthest vertex
    assert(length(L) == 6);  // Path should be 1->2->5->6->7->8
    moveFront(L);
    assert(get(L) == 1);
    moveNext(L);
    assert(get(L) == 2);
    moveNext(L);
    assert(get(L) == 5);
    moveNext(L);
    assert(get(L) == 6);
    moveNext(L);
    assert(get(L) == 7);
    moveNext(L);
    assert(get(L) == 8);
    clear(L);

    BFS(G, 2); // test running bfs a second time
    assert(getSource(G) == 2);
    assert(getDist(G, 5) == 1);   // testing distance
    assert(getDist(G, 8) == 4); 
    assert(getParent(G, 4) == 2); // testing parent
    assert(getParent(G, 3) == 1);
    
    // Test path finding with getPath()
    getPath(L, G, 8);  // Testing path to farthest vertex
    assert(length(L) == 5);  // Path should be 2->5->6->7->8
    moveFront(L);
    assert(get(L) == 2);
    moveNext(L);
    assert(get(L) == 5);
    moveNext(L);
    assert(get(L) == 6);
    moveNext(L);
    assert(get(L) == 7);
    moveNext(L);
    assert(get(L) == 8);
    clear(L);

    // test add arc
    D = newGraph(5);
    printGraph(stdout, D);
    addArc(D, 1, 2);
    addArc(D, 2, 3);
    addArc(D, 3, 4);
    addArc(D, 4, 5);
    printGraph(stdout, D);
    printf("\n");

    BFS(D, 1);
    assert(getDist(D, 5) == 4);  
    assert(getParent(D, 3) == 2); 

    addArc(D, 1, 3); 
    printGraph(stdout, D);
    printf("\n");

    BFS(D, 1);
    assert(getDist(D, 5) == 3); 
    assert(getParent(D, 3) == 1); 
    
    // Test makeNull() functionality
    makeNull(G);
    assert(getSize(G) == 0);
    assert(getOrder(G) == 8);  // Order should remain same
    printGraph(stdout, G);
    printf("\n");
    
    // Test adding edges after makeNull
    addEdge(G, 1, 2);
    assert(getSize(G) == 1);
    printGraph(stdout, G);
    printf("\n");
    
    // Clean up all allocated memory
    freeList(&L);
    freeGraph(&G);
    freeGraph(&D);

    printf("All tests passed!\n");
    return 0;
}
