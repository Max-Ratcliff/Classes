#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_

#include <stdbool.h>
#include "List.h"

#define INF -1
#define NIL 0 // all vertices are labeled from 1 to n making 0 invalid

typedef struct GraphObj* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n); // Creates and returns a new Graph with n vertices and no edges
void freeGraph(Graph* pG); // Frees memory associated with *pG, and sets *pG to NULL
/*** Access functions ***/
int getOrder(Graph G); // Returns the number of vertices in G
int getSize(Graph G); // Returns the number of edges in G
int getSource(Graph G); // returns the source vertex most recently used in function BFS(), or NIL if BFS() has not yet been called
int getParent(Graph G, int u); // return the parent of vertex 𝑢 in the BFS tree created by BFS(), or NIL if BFS() has not yet been called pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
int getDist(Graph G, int u); // returns the distance from the most recent BFS source to vertex 𝑢, or INF if BFS() has not yet been called pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
void getPath(List L, Graph G, int u); // appends to the List 𝐿 the vertices of a shortest path in 𝐺 from source to 𝑢, or appends to 𝐿 the value NIL if no such path exists. precondition getSource(G)!=NIL pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
/*** Manipulation procedures ***/
void makeNull(Graph G); // deletes all edges of 𝐺, restoring it to its original (no edge) state
void addEdge(Graph G, int u, int v); // inserts a new edge joining 𝑢 to 𝑣, i.e. 𝑢 is added to the adjacency List of 𝑣, and 𝑣 to the adjacency List of 𝑢. Your program is required to maintain these lists in sorted order by increasing labels pre: 1 ≤ 𝑢,𝑣 ≤ getOrder(𝐺)
void addArc(Graph G, int u, int v); // inserts a new directed edge from 𝑢 to 𝑣, i.e. 𝑣 is added to the adjacency List of 𝑢 (but not 𝑢 to the adjacency List of 𝑣) pre: 1 ≤ 𝑢,𝑣 ≤ getOrder(𝐺)
void BFS(Graph G, int s); // runs the BFS algorithm on the Graph 𝐺 with source 𝑠, setting the color, distance, parent, and source fields of 𝐺 accordingly.pre: 1 ≤ 𝑠 ≤ getOrder(𝐺)
/*** Other operations ***/
void printGraph(FILE* out, Graph G); // prints the adjacency list representation of 𝐺 to the file pointed to by out.

#endif