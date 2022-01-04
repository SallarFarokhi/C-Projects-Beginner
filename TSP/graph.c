#include "graph.h"

#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Note: A great deal of these functions were provided by
// Professor Long, in the assignment documentation.

// Defines the Graph struct and the values within it
// Provided by Professor Long
struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

// Assigns values in the graph
// Provided by Professor Long
Graph *graph_create(uint32_t vertices, bool undirected) {

    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

// Deletes the graph and frees the memory
// Provided by Professor Long
void graph_delete(Graph **G) {

    free(*G);
    *G = NULL;
    return;
}

// Returns the number of vertices
uint32_t graph_vertices(Graph *G) {

    return G->vertices;
}

// Adds an edge k from i to j
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {

    if (i > G->vertices || j > G->vertices) {
        return false;
    }
    G->matrix[i][j] = k;
    if (G->undirected) {
        G->matrix[j][i] = k;
    }
    return true;
}

// Checks if an edge exists from i to j
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {

    if (i > G->vertices || j > G->vertices) {
        return false;
    }
    return (G->matrix[i][j] > 0) ? true : false;
}

// Returns an edge weight from i to j
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {

    if (i > G->vertices || j > G->vertices) {
        return 0;
    }
    return G->matrix[i][j];
}

// Returns if a vertex has been visited
bool graph_visited(Graph *G, uint32_t v) {

    return G->visited[v];
}

// Marks a vertex as visited
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v <= G->vertices) {
        G->visited[v] = true;
    }
}

// Marks a vertex as unvisited
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v <= G->vertices) {
        G->visited[v] = false;
    }
}

// Prints the graph
void graph_print(Graph *G) {

    for (uint32_t x = 0; x < G->vertices; x += 1) {

        for (uint32_t y = 0; y < G->vertices; y += 1) {

            printf("%4" PRIu32, graph_edge_weight(G, x, y));

            // prints a new row after 5 values
            if ((y + 1) % G->vertices == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
}
