#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_

#include <stdbool.h>
#include "List.h"

#define INF -1
#define NIL 0 // all vertices are labeled from 1 to n making 0 invalid
#define UNDEF -1

typedef struct GraphObj* Graph;

/*** Constructors-Destructors ***/
Graph newGraph(int n); // Creates and returns a new Graph with n vertices and no edges
void freeGraph(Graph* pG); // Frees memory associated with *pG, and sets *pG to NULL
/*** Access functions ***/
int getOrder(Graph G); // Returns the number of vertices in G
int getSize(Graph G); // Returns the number of edges in G
int getParent(Graph G, int u); // return the parent of vertex 𝑢 in the BFS tree created by BFS(), or NIL if BFS() has not yet been called pre: 1 ≤ 𝑢 ≤ getOrder(𝐺)
int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
/*** Manipulation procedures ***/
void addEdge(Graph G, int u, int v); // inserts a new edge joining 𝑢 to 𝑣, i.e. 𝑢 is added to the adjacency List of 𝑣, and 𝑣 to the adjacency List of 𝑢. Your program is required to maintain these lists in sorted order by increasing labels pre: 1 ≤ 𝑢,𝑣 ≤ getOrder(𝐺)
void addArc(Graph G, int u, int v); // inserts a new directed edge from 𝑢 to 𝑣, i.e. 𝑣 is added to the adjacency List of 𝑢 (but not 𝑢 to the adjacency List of 𝑣) pre: 1 ≤ 𝑢,𝑣 ≤ getOrder(𝐺)
void DFS(Graph G, List S); /* will perform the depth first search algorithm on 𝐺 Pre: length(S)==getOrder(G) */
/*** Other operations ***/
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out, Graph G); // prints the adjacency list representation of 𝐺 to the file pointed to by out.

#endif