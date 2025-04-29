/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA2
* Graph.c 
* Implementation of Graph ADT with manipulation functions
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include "List.h"
#include "Graph.h"

typedef struct ListObj* List;
typedef struct GraphObj* Graph;

// #define INF -1
// #define NIL 0 // all vertices are labeled from 1 to n making 0 invalid

// structs -------------------------------------------------------------------

typedef struct GraphObj{
    List* adjacencyMat; // An array of Lists whose 𝑖th element contains the neighbors of vertex 𝑖.
    char* color; // An array of ints (or chars, or strings) whose 𝑖th element is the color (white, gray, black) of vertex 𝑖.
    int* parent; // An array of ints whose 𝑖th element is the parent of vertex 𝑖.
    int* distance; // An array of ints whose 𝑖th element is the distance from the (most recent) source to vertex 𝑖.
    int order; // number of vertices
    int size; // number of edges
    int source; // source vertex most recently used in function BFS(), or NIL if BFS() has not yet been called
}GraphObj;
// Constructors-Destructors ---------------------------------------------------

// Creates and returns a new Graph with n vertices and no edges
Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj)); // allocate memory for Graph
    G->adjacencyMat = malloc(sizeof(List) * (n + 1)); // allocate array of Lists
    G->color = malloc(sizeof(char) * (n + 1)); // allocate memory array of chars for colors (w, g, b)
    G->parent = malloc(sizeof(int) * (n + 1)); // allocate memory for int array of parents
    G->distance = malloc(sizeof(int) * (n + 1)); // allocate memory for int array of distance array
    G->order = n;
    G->size = 0;
    G->source = NIL;
    for (int i = 1; i < n + 1; i++) {
        G->adjacencyMat[i] = newList(); // create new List for each vertex
        G->color[i] = 'w'; // initialize color to white
        G->parent[i] = NIL; // initialize parent to NIL 
        G->distance[i] = INF; // initialize distance to INF
    }
    return G;
}

// Frees memory associated with *pG, and sets *pG to NULL
void freeGraph(Graph* pg){
    if(pg != NULL && *pg != NULL) {
        for (int i = 1; i < (*pg)->order + 1; i++) { // iterate from 1 to n+1
            freeList(&(*pg)->adjacencyMat[i]); // free each List
        }
        free((*pg)->adjacencyMat); // free array of Lists
        free((*pg)->color); // free color array
        free((*pg)->parent); // free parent array
        free((*pg)->distance); // free distance array
        free(*pg); // free Graph
        *pg = NULL;
    }
}

// Access functions -----------------------------------------------------------
int getOrder(Graph G) {
    assert(G != NULL);
    return G->order;
}

int getSize(Graph G) {
    assert(G != NULL);
    return G->size;
}

int getSource(Graph G) {
    assert(G != NULL);
    return G->source;
}

int getParent(Graph G, int u) {
    assert(G != NULL && 1 <= u && u <= getOrder(G));
    if (getSource(G)!=NIL){
        return G->parent[u];
    }
    return NIL;
}

int getDist(Graph G, int u) {
    assert(G != NULL && 1 <= u && u <= getOrder(G));
    if(getSource(G)!=NIL){
        return G->distance[u];
    }
    return INF;
}

void getPath(List L, Graph G, int u) {
    assert(getSource(G) != NIL && 1 <= u && u <= getOrder(G));

    if (getSource(G) == INF){
        append(L, NIL);
        return;
    }

    if(u == getSource(G)){ // if u is the source, append it
        append(L, u);
        return;
    } else if (getParent(G, u) == NIL) { // if u has no parent there is no shortest path
        append(L, NIL);
        return;
    } else {
        getPath(L, G, getParent(G, u)); // recursive call to get parent of u
        append(L, u); // append u to list
    }
}

// Manipulation procedures ----------------------------------------------------

void makeNull(Graph G) {
    assert(G != NULL);
    for (int i = 1; i < G->order + 1; i++) { // iterate from 1 to n+1
        clear(G->adjacencyMat[i]); // reset each list
    }
    G->size = 0;
}

// helper function to insertion sort a node
void insertSorted(List L, int x) {
    if (length(L) == 0) {
        append(L, x);
        return;
    }
    
    moveFront(L); // move cursor to front
    while (index(L) != -1) { // iterate through list
        if (x == get(L)) { // x is already in list
            return;
        }
        if (x < get(L)) { // x is bigger than cursor element
            insertBefore(L, x); // insert x before cursor
            return;
        }
        moveNext(L); // move cursor to next node
    }
    append(L, x); // x is the biggest element
}

void addEdge(Graph G, int u, int v){
    assert(G != NULL);
    assert(1 <= u && u <= getOrder(G));
    assert(1 <= v && v <= getOrder(G));
    insertSorted(G->adjacencyMat[u], v); // add v to u's adjacency list
    insertSorted(G->adjacencyMat[v], u); // add u to v's adjacency list
    G->size++;
}

void addArc(Graph G, int u, int v){
    assert(G != NULL);
    assert(1 <= u && u <= getOrder(G));
    assert(1 <= v && v <= getOrder(G));
    insertSorted(G->adjacencyMat[u], v); // add v to u's adjacency list
    G->size++;
}

void BFS(Graph G, int s){
    assert(G != NULL);
    assert(1 <= s && s <= getOrder(G));
    int x; // vertex x
    G->source = s;
    // BFS(𝐺, 𝑠)
    // 1. for 𝑥 ∈ V(G) − {s}
    // 2.   color[𝑥] = white
    // 3.   𝑑[𝑥] = ∞
    // 4.   𝑝[𝑥] = nil
    // 5. color[𝑠] = gray // discover the source 𝑠
    // 6. 𝑑[𝑠] = 0
    // 7. 𝑝[𝑠] = nil
    // 8. 𝑄 = ∅ // construct a new empty queue
    // 9. Enqueue(𝑄, 𝑠)
    // 10. while 𝑄 ≠ ∅
    // 11.  𝑥 = Dequeue(𝑄)
    // 12.  for 𝑦 ∈ adj[𝑥]
    // 13.      if color[𝑦] == white // 𝑦 is undiscovered
    // 14.          color[𝑦] = gray // discover 𝑦
    // 15.          𝑑[𝑦] = 𝑑[𝑥] + 1
    // 16.          𝑝[𝑦] = 𝑥
    // 17.          Enqueue(𝑄, 𝑦)
    // 18.  color[𝑥] = black // finish 𝑥
    for(x = 1; x < getOrder(G) + 1; x++) { // Reset Graph for a new BFS run
        G->color[x] = 'w'; // set color to white
        G->distance[x] = INF; // set distance to INF
        G->parent[x] = NIL; // set parent to NIL
    }
    G->color[s] = 'g'; // discover the source s
    G->distance[s] = 0; // set distance to 0
    G->parent[s] = NIL; // set parent to NIL
    List Q = newList(); // construct a new empty queue
    append(Q, s); // enqueue s
    while(length(Q) != 0) { // while 𝑄 ≠ ∅
        x = front(Q);
        deleteFront(Q); // dequeue x
        for(moveFront(G->adjacencyMat[x]); index(G->adjacencyMat[x]) >= 0; moveNext(G->adjacencyMat[x])) { // iterate through x's adjacecy matrix
            int y = get(G->adjacencyMat[x]); 
            if(G->color[y] == 'w') { // y is undiscovered
                G->color[y] = 'g'; // discover y
                G->distance[y] = G->distance[x] + 1; // distance from source to y is the same as source to x + 1
                G->parent[y] = x;
                append(Q, y); // enqueue y
            }
        }
        G->color[x]= 'b'; // finish discovering x
    }

    // clean up queue
    freeList(&Q);
}

void printGraph(FILE* out, Graph G){
    assert(G != NULL);
    for(int i = 1; i < getOrder(G) + 1; i++) {
        fprintf(out, "%d: ", i);
        printList(out, G->adjacencyMat[i]);
        fprintf(out, "\n");
    }
}

