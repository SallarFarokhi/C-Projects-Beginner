// Header files containing the ADTs
#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// command line arguments
#define OPTIONS "hvui:o:"

// prints out the shortest path, and recursive calls
void dfs(Graph *G, uint32_t v, Path *curr, Path *shr, bool ver, char *cities[], FILE *out) {

    // marks v as visited, and pushes it onto the stack
    path_push_vertex(curr, v, G);
    graph_mark_visited(G, v);
    static int recursive;

    // for all neighbors of v
    for (uint32_t i = 0; i < graph_vertices(G); i++) {
        if (graph_edge_weight(G, v, i) && !graph_visited(G, i)) {
            recursive += 1;
            // call dfs on that neighbor
            dfs(G, i, curr, shr, ver, cities, out);
        }
    }

    //          When there are no neighbors left:        //

    // temporary addresses for the pop
    uint32_t temp;
    uint32_t *temp_add = &temp;

    // is the current path a proper hamiltonian path?
    if (path_vertices(curr) == graph_vertices(G) && graph_edge_weight(G, v, START_VERTEX)) {
        path_push_vertex(curr, START_VERTEX, G);

        // does the user want all hamiltonian paths printed?
        if (ver) {
            path_print(curr, out, cities);
        }

        // is the current path shorter than the shortest path?
        if (path_length(curr) < path_length(shr) || path_length(shr) == 0) {
            path_copy(shr, curr);
        }
        path_pop_vertex(curr, temp_add, G);
    }

    // pops v from the path, and marks it as unvisited
    graph_mark_unvisited(G, v);
    path_pop_vertex(curr, temp_add, G);

    // if the function is done, print the shortest path
    if (!path_vertices(curr)) {
        path_print(shr, out, cities);
        fprintf(out, "Total recursive calls: %d\n", recursive);
        return;
    }
}

// tsp.c takes in information describing an adjacency matrix, through
// either command line arguments or standard input, and returns the
// shortest path, to either standard output or a seperate file.
int main(int argc, char **argv) {

    int opt = 0;

    // test values that correspond to the command line arguments
    bool help, verbose, undirected, input, output;

    // input and output files, defaulted to standard
    // Provided by Eugene, in the section video
    FILE *infile = stdin;
    FILE *outfile = stdout;

    struct Graph *graph;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'h': help = true; break;
        case 'v': verbose = true; break;
        case 'u': undirected = true; break;
        // Provided by Eugene, in the section video
        case 'i': {
            input = true;
            infile = fopen(optarg, "r");
            break;
        }
        case 'o': {
            output = true;
            outfile = fopen(optarg, "w");
            break;
        }
        default: {
            help = true;
            break;
        }
        }
    }

    // prints the help menu
    if (help) {
        printf("SYNOPSIS\n  Traveling Salesman Problem using DFS.\n\n");
        printf("USAGE\n  ./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n\n");
        printf("OPTIONS\n");
        printf("  -u             Use undirected graph.\n");
        printf("  -v             Enable verbose printing.\n");
        printf("  -h             Program usage and help.\n");
        printf("  -i infile      Input containing graph (default: stdin)\n");
        printf("  -o outfile     Output of computed path (default: stdout)\n");
        free(infile);
        free(outfile);
        return 0;
    }

    // stores the added graph values
    uint32_t i;
    uint32_t j;
    uint32_t weight;
    bool done = true;

    // stores city values and vertex count
    uint32_t vertex;
    char **city_name = (char **) calloc(28, sizeof(char *));
    char buffer[1024];

    // determines where the inputted values are coming from
    FILE *infile_values = (!input) ? stdin : infile;

    // takes in the number of vertices, with an error case
    char newline;
    fscanf(infile_values, "%d", &vertex);
    fscanf(infile_values, "%c", &newline);
    if (vertex > VERTICES) {
        fprintf(stderr, "Error: malformed number of vertices.\n");
        return 0;
    }

    // initializes the graph with the data given
    graph = graph_create(vertex, undirected);

    // inputs city names for the pathfinder
    for (uint32_t i = 0; i < vertex; i++) {
        fgets(buffer, sizeof(buffer), infile_values);

        // tests for and removes the newline, then stores the city name
        for (int j = 0; j < 1024; j++) {
            if (buffer[j] == '\n') {
                buffer[j] = '\0';
                city_name[i] = strdup(buffer);
                break;
            }
        }
    }

    // tests when the EOF is inputted, to end the loop
    while (done) {
        if (feof(infile_values)) {
            done = false;
            break;
        }

        // records the graph edge, and inputs it
        fscanf(infile_values, "%d %d %d", &i, &j, &weight);
        graph_add_edge(graph, i, j, weight);
        if (undirected) {
            graph_add_edge(graph, j, i, weight);
        }
    }

    // creates the paths
    struct Path *curr;
    struct Path *shortest;

    // initializes them
    curr = path_create();
    shortest = path_create();

    // runs the pathfinding, and prints to outfile
    dfs(graph, START_VERTEX, curr, shortest, verbose, city_name, outfile);

    // frees all of the data that was dynamically allocated
    for (uint32_t i = 0; i < 28; i++) {
        free(city_name[i]);
    }
    free(city_name);
    path_delete(&curr);
    path_delete(&shortest);
    graph_delete(&graph);
    fclose(infile);
    fclose(outfile);

    // end of program
    return 0;
}
