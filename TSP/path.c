#include "path.h"

#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Provided by Professor Long
struct Path {
    Stack *vertices;
    uint32_t length;
};

// Creates the path and returns the pointer
Path *path_create(void) {

    Path *p = (Path *) malloc(sizeof(Path));
    if (p) {
        p->vertices = stack_create(VERTICES);
        p->length = 0;
        if (!p->vertices) {
            free(p);
            p = NULL;
        }
    }
    return p;
}

// Deletes the path and the pointer
void path_delete(Path **p) {

    if (*p && (*p)->vertices) {
        stack_delete(&(*p)->vertices);
        free((*p)->vertices);
        free(*p);
        *p = NULL;
    }
}

// Pushes a value onto the path
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {

    // temporary address to stack_peek
    uint32_t temp_value;
    uint32_t *temp_address = &temp_value;

    if (path_vertices(p) != 0) {
        if (!stack_peek(p->vertices, temp_address)) {
            return false;
        }
    }
    if (!stack_push(p->vertices, v)) {
        return false;
    }
    if (path_vertices(p)) {
        p->length += graph_edge_weight(G, temp_value, v);
    }
    return true;
}

// Takes a value from the path
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {

    // temporary address for stack_peek
    uint32_t temp_value;
    uint32_t *temp_address = &temp_value;

    if (!stack_pop(p->vertices, v)) {
        return false;
    }
    if (path_vertices(p)) {
        if (!stack_peek(p->vertices, temp_address)) {
            return false;
        }
        p->length -= graph_edge_weight(G, temp_value, *v);
    }
    return true;
}

// Returns the number of vertices
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

// Returns the length of the path
uint32_t path_length(Path *p) {
    return p->length;
}

// Copies the path into another path
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}

// Prints the path to a file
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %d\n", p->length);
    fprintf(outfile, "Path: ");
    // prints the stack
    stack_print(p->vertices, outfile, cities);
}
