#include "graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    g->vertices = vertices;
    g->directed = directed;

    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));

    g->weights = calloc(vertices, sizeof(g->weights[0]));

    for (uint32_t i = 0; i < vertices; i++) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }

    return g;
}

void graph_free(Graph **gp) {
    //free weights and names
    for (uint32_t i = 0; i < (*gp)->vertices; i++) {
        free((*gp)->weights[i]);
        free((*gp)->names[i]);
    }
    free((*gp)->weights);
    free((*gp)->names);

    free((*gp)->visited);
    free(*gp);
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    return (g->vertices);
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g->names[v])
        free(g->names[v]);
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    assert(g->vertices > v);
    return (g->names[v]);
}

char **graph_get_names(const Graph *g) {
    return (g->names);
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    if (g->directed) { //directed graph
        g->weights[start][end] = weight;
    } else { //undirected graph
        g->weights[start][end] = weight;
        g->weights[end][start] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    return (g->weights[start][end]);
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    return g->visited[v];
}

void graph_print(const Graph *g) {
    printf("%u\n", g->vertices);
    for (uint32_t i = 0; i < g->vertices; i++) {
        printf("%s\n", g->names[i]);
    }
    for (uint32_t i = 0; i < g->vertices; i++) {
        for (uint32_t j = 0; j < g->vertices; j++) {
            printf("%u ", g->weights[i][j]);
        }
        printf("\n");
    }
}
