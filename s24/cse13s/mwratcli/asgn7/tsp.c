#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "i:o:dh"
#define USAGE                                                                                      \
    "Usage: tsp [options]\n\n"                                                                     \
    "-i infile    Specify the input file path containing the cities and edges\n"                   \
    "             of a graph. If not specified, the default input should be\n"                     \
    "             set as stdin.\n\n"                                                               \
    "-o outfile   Specify the output file path to print to. If not specified,\n"                   \
    "             the default output should be set as stdout.\n\n"                                 \
    "-d           Specifies the graph to be directed.\n\n"                                         \
    "-h           Prints out a help message describing the purpose of the\n"                       \
    "             graph and the command-line options it accepts, exiting the\n"                    \
    "             program afterwards.\n"

void dfs(Graph *g, uint32_t v, Path *curr, Path *best) {
    //add the path to the top of the stack and mark the vertex as visited
    path_add(curr, v, g);
    graph_visit_vertex(g, v);
    if (path_vertices(curr) == graph_vertices(g)) { //check if we traverse every path
        if (graph_get_weight(g, v, 0) != 0) { //if the path has a weight add it to curr
            path_add(curr, 0, g);

            if (path_distance(best) == 0
                || path_distance(curr) < path_distance(
                       best)) { // if the distance of curr is smaller than the distance of best or best hasnt been initialized copy curr into best
                path_copy(best, curr);
            }
            path_remove(curr, g); //then pop the path we just added
        }
    }
    for (uint32_t node = 0; node < graph_vertices(g); node++) { //dfs alg... search down every path
        if (graph_get_weight(g, v, node) > 0) {
            if (!graph_visited(g, node)) {
                dfs(g, node, curr, best);
            }
        }
    }
    graph_unvisit_vertex(g, v);
    path_remove(curr, g);
}

int main(int argc, char *argv[]) {
    int opt;

    bool directed = false;

    FILE *outfile = stdout;

    FILE *infile = stdin;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                infile = stdin;
                fprintf(stderr, USAGE);
            }
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                outfile = stdout;
                fprintf(stderr, USAGE);
            }
            break;
        case 'd': directed = true; break;
        case 'h': fprintf(stdout, USAGE); break;
        }
    }

    //start building graph
    uint32_t vertices;
    if (fscanf(infile, "%u", &vertices) != 1) {
        fprintf(stderr, "tsp: error reading number of vertices\n");
        fclose(infile);
        exit(1);
    }
    //capture newline
    char cap[10];
    fgets(cap, 10, infile);
    Graph *g = graph_create(vertices, directed);
    char *name = (char *) malloc(1024);
    for (uint32_t i = 0; i < vertices; i++) {
        if (!fgets(name, 1024, infile)) {
            fprintf(stderr, "tsp: error reading vertex %u's name read %s\n", i, name);
            fclose(infile);
            exit(1);
        }
        //capture newline
        name[strcspn(name, "\n")] = '\0';
        graph_add_vertex(g, name, i);
    }
    uint32_t edges;
    if (fscanf(infile, "%u", &edges) != 1) {
        fprintf(stderr, "tsp: error reading number of edges, read: %u\n", edges);
        fclose(infile);
        exit(1);
    }
    for (uint32_t i = 0; i < edges; i++) {
        uint32_t start, end, weight;
        if (fscanf(infile, "%u %u %u", &start, &end, &weight) != 3) {
            fprintf(stderr, "tsp: error reading edge %u\n", i);
            fclose(infile);
            exit(1);
        }
        graph_add_edge(g, start, end, weight);
    }

    //initialize paths
    Path *curr = path_create(vertices + 1);
    Path *best = path_create(vertices + 1);
    dfs(g, START_VERTEX, curr, best);
    //best should now contain the best path

    if (path_distance(best) == 0) {
        fprintf(outfile, "No path found! Alissa is lost!\n");
    } else {
        //print path
        fprintf(outfile, "Alissa starts at:\n");
        path_print(best, outfile, g);
    }

    free(name);
    path_free(&curr);
    path_free(&best);
    graph_free(&g);
    if (infile != stdin) {
        fclose(infile);
    }
    if (outfile != stdout) {
        fclose(outfile);
    }

    return 0;
}
